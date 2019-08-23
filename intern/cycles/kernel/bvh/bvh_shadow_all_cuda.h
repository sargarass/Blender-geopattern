/*
 * Adapted from code Copyright 2009-2010 NVIDIA Corporation,
 * and code copyright 2009-2012 Intel Corporation
 *
 * Modifications Copyright 2011-2013, Blender Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef __QBVH__
#  include "kernel/bvh/qbvh_shadow_all.h"
#endif

#if BVH_FEATURE(BVH_HAIR)
#  define NODE_INTERSECT bvh_node_intersect
#else
#  define NODE_INTERSECT bvh_aligned_node_intersect
#endif

#include <kernel/kernel_types.h>
#include "bvh_traversal_cuda_types.h"
/* This is a template BVH traversal function, where various features can be
 * enabled/disabled. This way we can compile optimized versions for each case
 * without new features slowing things down.
 *
 * BVH_INSTANCING: object instancing
 * BVH_HAIR: hair curve rendering
 * BVH_MOTION: motion blur rendering
 *
 */

#ifndef __KERNEL_GPU__
ccl_device
#else
ccl_device_inline
#endif
bool BVH_FUNCTION_FULL_NAME(BVH)(KernelGlobals *kg,
                                 const Ray *ray,
                                 Intersection *isect_array,
                                 const int skip_object,
                                 const uint max_hits,
                                 uint *num_hits)
{
	/* todo:
	 * - likely and unlikely for if() statements
	 * - test restrict attribute for pointers
	 */

	/* traversal stack in CUDA thread-local memory */
	int traversal_stack[BVH_STACK_SIZE];
	traversal_stack[0] = ENTRYPOINT_SENTINEL;

	/* traversal variables in registers */
	int stack_ptr = 0;
	int node_addr = kernel_data.bvh.root;

	/* ray parameters in registers */
	const float tmax = ray->t;
	float3 P = ray->P;
	float3 dir = bvh_clamp_direction(ray->D);
	float3 idir = bvh_inverse_direction(dir);
	int object = OBJECT_NONE;
	int  entry_prim = GEOPATTERN_NO_LINK;
	int  entry_obj = GEOPATTERN_NO_LINK;
	float isect_t = tmax;
    float t_far = FLT_MAX;
    float t_near = -FLT_MAX;
#if BVH_FEATURE(BVH_MOTION)
	Transform ob_itfm;
#endif

#if BVH_FEATURE(BVH_INSTANCING)
	int num_hits_in_instance = 0;
#endif
	int num_hits_in_geopattern = 0;
	GeopatternFrame geoframe;
	memset(&geoframe, 0, sizeof(GeopatternFrame));

	*num_hits = 0;
	isect_array->t = tmax;

	/* traversal loop */
	do {
		do {
			/* traverse internal nodes */
			while(node_addr >= 0 && node_addr != ENTRYPOINT_SENTINEL) {
				int node_addr_child1, traverse_mask;
				float dist[2];
				float4 cnodes = kernel_tex_fetch(__bvh_nodes, node_addr+0);

				traverse_mask = NODE_INTERSECT(kg,
				                               P,
#  if BVH_FEATURE(BVH_HAIR)
				                               dir,
#  endif
				                               idir,
				                               isect_t,
				                               node_addr,
				                               PATH_RAY_SHADOW,
				                               dist,
                                               t_near,
                                               t_far);

				node_addr = __float_as_int(cnodes.z);
				node_addr_child1 = __float_as_int(cnodes.w);

				if(traverse_mask == 3) {
					/* Both children were intersected, push the farther one. */
					bool is_closest_child1 = (dist[1] < dist[0]);
					if(is_closest_child1) {
						int tmp = node_addr;
						node_addr = node_addr_child1;
						node_addr_child1 = tmp;
					}

					++stack_ptr;
					kernel_assert(stack_ptr < BVH_STACK_SIZE);
					traversal_stack[stack_ptr] = node_addr_child1;
				}
				else {
					/* One child was intersected. */
					if(traverse_mask == 2) {
						node_addr = node_addr_child1;
					}
					else if(traverse_mask == 0) {
						/* Neither child was intersected. */
						node_addr = traversal_stack[stack_ptr];
						--stack_ptr;
					}
				}
			}

			/* if node is leaf, fetch triangle list */
			if(node_addr < 0) {
				float4 leaf = kernel_tex_fetch(__bvh_leaf_nodes, (-node_addr-1));
				int prim_addr = __float_as_int(leaf.x);

#if BVH_FEATURE(BVH_INSTANCING)
				if(prim_addr >= 0) {
#endif
					const int prim_addr2 = __float_as_int(leaf.y);
					const uint type = __float_as_int(leaf.w);
					const uint p_type = type & PRIMITIVE_ALL;

					/* pop */
					node_addr = traversal_stack[stack_ptr];
					--stack_ptr;

					/* primitive intersection */
					while(prim_addr < prim_addr2) {
						kernel_assert((kernel_tex_fetch(__prim_type, prim_addr) & PRIMITIVE_ALL) == p_type);

#ifdef __SHADOW_TRICKS__
						uint tri_object = (object == OBJECT_NONE)
						        ? kernel_tex_fetch(__prim_object, prim_addr)
						        : object;
						if(tri_object == skip_object) {
							++prim_addr;
							continue;
						}
#endif

						bool hit = false;

						/* todo: specialized intersect functions which don't fill in
						 * isect unless needed and check SD_HAS_TRANSPARENT_SHADOW?
						 * might give a few % performance improvement */

						switch(p_type) {
							case PRIMITIVE_TRIANGLE: {
#ifndef __SHADOW_TRICKS__
                                int object_idx = ((object == OBJECT_NONE)? kernel_tex_fetch(__prim_object, prim_addr) : object);
#else
								int object_idx = tri_object;
#endif
								const float4 geopattern_settings = kernel_tex_fetch(__object_geopattern, object_idx);
								const uint geopattern_link = __float_as_uint(geopattern_settings.x);
								const uint geopattern_clipbox_link = __float_as_uint(geopattern_settings.y);
								const float geopatten_height = geopattern_settings.z;
								const uint geopattern_flags = __float_as_uint(geopattern_settings.w);
								//assert((object == OBJECT_NONE)? true : (geopattern_link == GEOPATTERN_NO_LINK));
								if (entry_obj == object_idx || geopattern_link == GEOPATTERN_NO_LINK) {
									hit = triangle_intersect(kg,
															 isect_array,
															 P,
															 dir,
															 PATH_RAY_SHADOW,
															 object,
															 prim_addr,
                                                             t_near,
                                                             t_far);

//									if (hit) {
//										isect_array->entry_object = entry_obj;
//										isect_array->entry_prim = entry_prim;
//									}

								} else {
									if (common_part_geopattern_inside(kg, prim_addr, object_idx, geopattern_link, geopattern_clipbox_link, geopatten_height, geopattern_flags,
																	  P, dir, idir, isect_t, t_near, t_far,
																	  stack_ptr, traversal_stack, node_addr, object,
																	  geoframe, isect_array)) {
										const int prim = kernel_tex_fetch(__prim_index, prim_addr);
										entry_prim = prim;
										entry_obj = object_idx;
										num_hits_in_geopattern = 0;
										isect_array->t = isect_t;
									}
								}
								break;
							}
#if BVH_FEATURE(BVH_MOTION)
							case PRIMITIVE_MOTION_TRIANGLE: {
								hit = motion_triangle_intersect(kg,
								                                isect_array,
								                                P,
								                                dir,
								                                ray->time,
								                                PATH_RAY_SHADOW,
								                                object,
								                                prim_addr,
								                                t_near,
								                                t_far);
								break;
							}
#endif
#if BVH_FEATURE(BVH_HAIR)
							case PRIMITIVE_CURVE:
							case PRIMITIVE_MOTION_CURVE: {
								const uint curve_type = kernel_tex_fetch(__prim_type, prim_addr);
								if(kernel_data.curve.curveflags & CURVE_KN_INTERPOLATE) {
									hit = bvh_cardinal_curve_intersect(kg,
									                                   isect_array,
									                                   P,
									                                   dir,
									                                   PATH_RAY_SHADOW,
									                                   object,
									                                   prim_addr,
									                                   ray->time,
									                                   curve_type,
									                                   NULL,
									                                   0, 0,
									                                   t_near,
								                                       t_far);
								}
								else {
									hit = bvh_curve_intersect(kg,
									                          isect_array,
									                          P,
									                          dir,
									                          PATH_RAY_SHADOW,
									                          object,
									                          prim_addr,
									                          ray->time,
									                          curve_type,
									                          NULL,
									                          0, 0,
									                          t_near,
									                          t_far);
								}
								break;
							}
#endif
							default: {
								hit = false;
								break;
							}
						}

						/* shadow ray early termination */
						if(hit) {
							/* detect if this surface has a shader with transparent shadows */

							/* todo: optimize so primitive visibility flag indicates if
							 * the primitive has a transparent shadow shader? */
							int prim = kernel_tex_fetch(__prim_index, isect_array->prim);
							int shader = 0;

#ifdef __HAIR__
							if(kernel_tex_fetch(__prim_type, isect_array->prim) & PRIMITIVE_ALL_TRIANGLE)
#endif
							{
								shader = kernel_tex_fetch(__tri_shader, prim);
							}
#ifdef __HAIR__
							else {
								float4 str = kernel_tex_fetch(__curves, prim);
								shader = __float_as_int(str.z);
							}
#endif
							int flag = kernel_tex_fetch(__shader_flag, (shader & SHADER_MASK)*SHADER_SIZE);

							/* if no transparent shadows, all light is blocked */
							if(!(flag & SD_HAS_TRANSPARENT_SHADOW)) {
								return true;
							}
							/* if maximum number of hits reached, block all light */
							else if(*num_hits == max_hits) {
								return true;
							}

							/* move on to next entry in intersections array */
							isect_array++;
							(*num_hits)++;
#if BVH_FEATURE(BVH_INSTANCING)
							num_hits_in_instance++;
#endif
							num_hits_in_geopattern++;

							isect_array->t = isect_t;
						}

						prim_addr++;
					}
				}
#if BVH_FEATURE(BVH_INSTANCING)
				else {
					/* instance push */
					int prev_object = object;
					object = kernel_tex_fetch(__prim_object, -prim_addr-1);

#  if BVH_FEATURE(BVH_MOTION)
					isect_t = bvh_instance_motion_push(kg, object, ray, &P, &dir, &idir, isect_t, &ob_itfm);
#  else
					isect_t = bvh_instance_push(kg, object, ray, &P, &dir, &idir, isect_t);
#  endif

					num_hits_in_instance = 0;
					isect_array->t = isect_t;

 					++stack_ptr;
                    kernel_assert(stack_ptr < BVH_STACK_SIZE);
                    traversal_stack[stack_ptr] = prev_object;

					++stack_ptr;
                    assert(stack_ptr < BVH_STACK_SIZE);
                    traversal_stack[stack_ptr] = INSTANCE_SENTINEL;

					node_addr = kernel_tex_fetch(__object_node, object);
				}
			}
#endif  /* FEATURE(BVH_INSTANCING) */
		} while(!IS_SENTINEL(node_addr));
		if (stack_ptr > 0) {
			if (node_addr == GEOPATTERN_SENTINEL) {
				int recovered_object = traversal_stack[stack_ptr];
				--stack_ptr;
				assert(stack_ptr >= 0);

				if(num_hits_in_geopattern) {
					for(int i = 0; i < num_hits_in_geopattern; i++) {
						(isect_array-i-1)->t = ((isect_array-i-1)->t - offset_t) / geoframe.t_mult + geoframe.t_min;
					}
				}

                P = geoframe.P;
                dir = geoframe.dir;
                idir = bvh_inverse_direction(dir);
                t_far = geoframe.t_far;
                t_near = geoframe.t_near;

				isect_t = tmax;
				isect_array->t = isect_t;

				entry_obj = GEOPATTERN_NO_LINK;
				entry_prim = GEOPATTERN_NO_LINK;
				num_hits_in_geopattern = 0;

				object = recovered_object;
				node_addr = traversal_stack[stack_ptr];
				--stack_ptr;
			}
#if BVH_FEATURE(BVH_INSTANCING)
			if (node_addr == INSTANCE_SENTINEL) {
				int recovered_object = traversal_stack[stack_ptr];
				--stack_ptr;
				assert(stack_ptr >= 0);
                kernel_assert(object != OBJECT_NONE);

                /* Instance pop. */
                if(num_hits_in_instance) {
                    float t_fac;

#  if BVH_FEATURE(BVH_MOTION)
                    bvh_instance_motion_pop_factor(kg, object, ray, &P, &dir, &idir, &t_fac, &ob_itfm);
#  else
                    bvh_instance_pop_factor(kg, object, ray, &P, &dir, &idir, &t_fac);
#  endif
                    /* scale isect->t to adjust for instancing */
                    for(int i = 0; i < num_hits_in_instance; i++) {
                        (isect_array-i-1)->t *= t_fac;
                    }
                }
                else {
#  if BVH_FEATURE(BVH_MOTION)
                    bvh_instance_motion_pop(kg, object, ray, &P, &dir, &idir, FLT_MAX, &ob_itfm);
#  else
                    bvh_instance_pop(kg, object, ray, &P, &dir, &idir, FLT_MAX);
#  endif
                }

                isect_t = tmax;
                isect_array->t = isect_t;

                object = recovered_object;
				node_addr = traversal_stack[stack_ptr];
				--stack_ptr;
            }
#endif  /* FEATURE(BVH_INSTANCING) */
		}
	} while(node_addr != ENTRYPOINT_SENTINEL);

	return false;
}

ccl_device_inline bool BVH_FUNCTION_NAME(KernelGlobals *kg,
                                         const Ray *ray,
                                         Intersection *isect_array,
                                         const int skip_object,
                                         const uint max_hits,
                                         uint *num_hits)
{
    #ifdef __QBVH__
	if(kernel_data.bvh.use_qbvh) {
		return BVH_FUNCTION_FULL_NAME(QBVH)(kg,
		                                    ray,
		                                    isect_array,
		                                    skip_object,
		                                    max_hits,
		                                    num_hits);
	}
	else
#endif
	{
		kernel_assert(kernel_data.bvh.use_qbvh == false);
		return BVH_FUNCTION_FULL_NAME(BVH)(kg,
		                                   ray,
		                                   isect_array,
		                                   skip_object,
		                                   max_hits,
		                                   num_hits);
	}
}

#undef BVH_FUNCTION_NAME
#undef BVH_FUNCTION_FEATURES
#undef NODE_INTERSECT
