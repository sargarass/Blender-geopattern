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
#  include "kernel/bvh/qbvh_traversal.h"
#endif

#if BVH_FEATURE(BVH_HAIR)
#  define NODE_INTERSECT bvh_node_intersect
#  define NODE_INTERSECT_ROBUST bvh_node_intersect_robust
#else
#  define NODE_INTERSECT bvh_aligned_node_intersect
#  define NODE_INTERSECT_ROBUST bvh_aligned_node_intersect_robust
#endif

/* This is a template BVH traversal function, where various features can be
 * enabled/disabled. This way we can compile optimized versions for each case
 * without new features slowing things down.
 *
 * BVH_INSTANCING: object instancing
 * BVH_HAIR: hair curve rendering
 * BVH_HAIR_MINIMUM_WIDTH: hair curve rendering with minimum width
 * BVH_MOTION: motion blur rendering
 *
 */
#include <kernel/kernel_types.h>
#include "bvh_traversal_cuda_types.h"
//#define USE_4_INTERSECTIONS

ccl_device_noinline bool BVH_FUNCTION_FULL_NAME(BVH)(KernelGlobals *kg,
                                                     const Ray *ray,
                                                     Intersection *isect,
                                                     const uint visibility
#if BVH_FEATURE(BVH_HAIR_MINIMUM_WIDTH)
                                                     , uint *lcg_state,
                                                     float difl,
                                                     float extmax
#endif
                                                     )
{
	/* todo:
	 * - test if pushing distance on the stack helps (for non shadow rays)
	 * - separate version for shadow rays
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
	float3 P = ray->P;
	float3 dir = bvh_clamp_direction(ray->D);
	float3 idir = bvh_inverse_direction(dir);
	float prev_t = 0.0f;

	int object = OBJECT_NONE;

#if BVH_FEATURE(BVH_MOTION)
	Transform ob_itfm;
#endif

	isect->t = ray->t;
	isect->u = 0.0f;
	isect->v = 0.0f;
	isect->prim = PRIM_NONE;
	isect->object = OBJECT_NONE;

	int entry_prim = GEOPATTERN_NO_LINK;
	int entry_obj = GEOPATTERN_NO_LINK;
	bool has_intersection_in_geopattern = false;
#ifndef USE_4_INTERSECTIONS
	GeopatternFrame geoframe;
	memset(&geoframe, 0, sizeof(GeopatternFrame));
#else
	GeopatternFrame2 geoframe2;
	memset(&geoframe2, 0, sizeof(GeopatternFrame));
#endif

	float t_far = FLT_MAX;
	float t_near = -FLT_MAX;
//    HitData_2 min_hit;
//    HitData_2 max_hit;
//    float3 tri_a;
//    float3 tri_b;
//    float3 tri_c;
//    float3 tri_n_a;
//    float3 tri_n_b;
//    float3 tri_n_c;
//    float3 bbox_min, bbox_max;
//    float cur_geopatten_height = 1.0f;
//    Mat3 Q;
//    Mat3 QQ;
	BVH_DEBUG_INIT();

	/* traversal loop */
	size_t thread_idx = getGlobalIdx3DZXY();
	do {
		do {
			/* traverse internal nodes */
			while(node_addr >= 0 && !IS_SENTINEL(node_addr)) {
				int node_addr_child1, traverse_mask;
				float dist[2];
				float4 cnodes = kernel_tex_fetch(__bvh_nodes, node_addr+0);

#  if BVH_FEATURE(BVH_HAIR_MINIMUM_WIDTH)
				if(difl != 0.0f) {
					traverse_mask = NODE_INTERSECT_ROBUST(kg,
					                                      P,
#    if BVH_FEATURE(BVH_HAIR)
					                                      dir,
#    endif
					                                      idir,
					                                      isect->t,
					                                      difl,
					                                      extmax,
					                                      node_addr,
					                                      visibility,
					                                      dist,
					                                      t_near,
					                                      t_far);
				}
				else
#  endif
				{
					traverse_mask = NODE_INTERSECT(kg,
					                               P,
#    if BVH_FEATURE(BVH_HAIR)
					                               dir,
#    endif
					                               idir,
					                               isect->t,
					                               node_addr,
					                               visibility,
					                               dist,
												   t_near,
					                               t_far);
				}

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
				BVH_DEBUG_NEXT_NODE();
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

					/* pop */
					node_addr = traversal_stack[stack_ptr];
					--stack_ptr;

					/* primitive intersection */
					switch(type & PRIMITIVE_ALL) {
						case PRIMITIVE_TRIANGLE: {
							for(; prim_addr < prim_addr2; prim_addr++) {
								assert((prim_addr2 - prim_addr) == 1);
								BVH_DEBUG_NEXT_INTERSECTION();
								kernel_assert(kernel_tex_fetch(__prim_type, prim_addr) == type);

								int object_idx = ((object == OBJECT_NONE)? kernel_tex_fetch(__prim_object, prim_addr) : object);
								const float4 geopattern_settings = kernel_tex_fetch(__object_geopattern, object_idx);

								const uint geopattern_link = __float_as_uint(geopattern_settings.x);
								const uint geopattern_clipbox_link = __float_as_uint(geopattern_settings.y);
								const float geopatten_height = geopattern_settings.z;
								const uint geopattern_flags = __float_as_uint(geopattern_settings.w);
								//assert((object == OBJECT_NONE)? true : (geopattern_link == GEOPATTERN_NO_LINK));
								if (entry_obj == object_idx || geopattern_link == GEOPATTERN_NO_LINK) {

								    int obj = (entry_obj != GEOPATTERN_NO_LINK)? entry_obj : object;

									if (triangle_intersect(kg,
														   isect,
														   P,
														   dir,
														   visibility,
                                                           obj,
														   prim_addr,
														   t_near,
														   t_far)) {
										/* shadow ray early termination */
										has_intersection_in_geopattern = true;

                                        if (visibility == PATH_RAY_SHADOW_OPAQUE)
											return true;
									}
								} else {
#ifndef USE_4_INTERSECTIONS
									prev_t = isect->t;
									if (common_part_geopattern_inside(kg, prim_addr, object_idx, geopattern_link, geopattern_clipbox_link, geopatten_height, geopattern_flags,
																	  P, dir, idir, isect->t, t_near, t_far,
																	  stack_ptr,traversal_stack, node_addr, object,
																	  geoframe, isect)) {

										const int prim = kernel_tex_fetch(__prim_index, prim_addr);
										entry_prim = prim;
										entry_obj = object_idx;
										has_intersection_in_geopattern = false;
									}
#else
									if (common_part_geopattern_inside_many_rays(prim_addr, object_idx,
											geopattern_link, geopattern_clipbox_link, geopatten_height, geopattern_flags,
											P, dir, t_near, t_far,
											object, stack_ptr, traversal_stack, node_addr, geoframe2,isect)) {
										const int prim = kernel_tex_fetch(__prim_index, prim_addr);
										entry_prim = prim;
										entry_obj = geoframe2.object;
										has_intersection_in_geopattern = false;
									}
#endif
								}
							}
							break;
						}
#if BVH_FEATURE(BVH_MOTION)
						case PRIMITIVE_MOTION_TRIANGLE: {
							for(; prim_addr < prim_addr2; prim_addr++) {
								BVH_DEBUG_NEXT_INTERSECTION();
								kernel_assert(kernel_tex_fetch(__prim_type, prim_addr) == type);
								if(motion_triangle_intersect(kg,
								                             isect,
								                             P,
								                             dir,
								                             ray->time,
								                             visibility,
								                             object,
								                             prim_addr,
								                             t_near,
								                             t_far))
								{
									isect->entry_object = entry_obj;
									isect->entry_prim = entry_prim;
									/* shadow ray early termination */
									if(visibility == PATH_RAY_SHADOW_OPAQUE)
										return true;
								}
							}
							break;
						}
#endif  /* BVH_FEATURE(BVH_MOTION) */
#if BVH_FEATURE(BVH_HAIR)
						case PRIMITIVE_CURVE:
						case PRIMITIVE_MOTION_CURVE: {
							for(; prim_addr < prim_addr2; prim_addr++) {
								BVH_DEBUG_NEXT_INTERSECTION();
								const uint curve_type = kernel_tex_fetch(__prim_type, prim_addr);
								kernel_assert((curve_type & PRIMITIVE_ALL) == (type & PRIMITIVE_ALL));
								bool hit;
								if(kernel_data.curve.curveflags & CURVE_KN_INTERPOLATE) {
									hit = bvh_cardinal_curve_intersect(kg,
									                                   isect,
									                                   P,
									                                   dir,
									                                   visibility,
									                                   object,
									                                   prim_addr,
									                                   ray->time,
									                                   curve_type,
									                                   lcg_state,
									                                   difl,
									                                   extmax,
									                                   t_near,
									                                   t_far);
								}
								else {
									hit = bvh_curve_intersect(kg,
									                          isect,
									                          P,
									                          dir,
									                          visibility,
									                          object,
									                          prim_addr,
									                          ray->time,
									                          curve_type,
									                          lcg_state,
									                          difl,
									                          extmax,
					                               			  t_near,
					                               			  t_far);
								}
								if(hit) {
									/* shadow ray early termination */
									if(visibility == PATH_RAY_SHADOW_OPAQUE)
										return true;
								}
							}
							break;
						}
#endif  /* BVH_FEATURE(BVH_HAIR) */
					}
				}
#if BVH_FEATURE(BVH_INSTANCING)
				else {
					/* instance push */
					int prev_object = object;
					object = kernel_tex_fetch(__prim_object, -prim_addr-1);

#  if BVH_FEATURE(BVH_MOTION)
					isect->t = bvh_instance_motion_push(kg, object, ray, &P, &dir, &idir, isect->t, &ob_itfm);
#  else
					isect->t = bvh_instance_push(kg, object, ray, &P, &dir, &idir, isect->t);
#  endif
					 ++stack_ptr;
                    kernel_assert(stack_ptr < BVH_STACK_SIZE);
                    traversal_stack[stack_ptr] = prev_object;

					++stack_ptr;
                    assert(stack_ptr < BVH_STACK_SIZE);
                    traversal_stack[stack_ptr] = INSTANCE_SENTINEL;
					node_addr = kernel_tex_fetch(__object_node, object);

					BVH_DEBUG_NEXT_INSTANCE();
				}
			}
#endif  /* FEATURE(BVH_INSTANCING) */
		} while (!IS_SENTINEL(node_addr));
		if (stack_ptr > 0) {
			if (node_addr == GEOPATTERN_SENTINEL) {
#ifndef USE_4_INTERSECTIONS
				int recovered_object = traversal_stack[stack_ptr];
				--stack_ptr;
				assert(stack_ptr >= 0);

				if (has_intersection_in_geopattern) {
					isect->entry_prim = entry_prim;
					isect->entry_object = entry_obj;

					int sd_prim = kernel_tex_fetch(__prim_index, isect->prim);
					const uint4 sd_tri_vindex = kernel_tex_fetch(__tri_vindex, sd_prim);
					const float3 v0 = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, sd_tri_vindex.w + 0));
					const float3 v1 = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, sd_tri_vindex.w + 1));
					const float3 v2 = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, sd_tri_vindex.w + 2));
					int object_flag = kernel_tex_fetch(__object_flag, entry_obj);
					float3 pp = dir * isect->t + P;

					float3 I = -dir;
					float3 N = triangle_normal(v0, v1, v2, object_flag);

					if (dot(I, N) < 0.0f) {
						N = -N;
					}

					float3 offset_up = my_ray_offset(pp, N);
					float3 offset_down = my_ray_offset(pp, -N);
//
					const uint4 tri_vindex = kernel_tex_fetch(__tri_vindex, entry_prim);
					float3 tri_a = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 0));
					float3 tri_b = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 1));
					float3 tri_c = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 2));
					float3 tri_n_a = geoframe.height *
									 normalize(float4_to_float3(kernel_tex_fetch(__tri_vnormal, tri_vindex.x)));
					float3 tri_n_b = geoframe.height *
									 normalize(float4_to_float3(kernel_tex_fetch(__tri_vnormal, tri_vindex.y)));
					float3 tri_n_c = geoframe.height *
									 normalize(float4_to_float3(kernel_tex_fetch(__tri_vnormal, tri_vindex.z)));
					float2 point = make_float2(pp.x, pp.y);
					float u, v;

					isect->offset_up = inverse_mapping(tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c, geoframe, offset_up);
					isect->offset_down = inverse_mapping(tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c, geoframe, offset_down);

					barycentric_float2(geoframe.A, geoframe.B, geoframe.C, point, u, v);
					float w = clamp((pp.z - geoframe.bbox_min_z) / (geoframe.bbox_max_z - geoframe.bbox_min_z), 0.0f, 1.0f);


					const float3 T_a = device_lerp(tri_a, tri_a + tri_n_a, w);
					const float3 T_b = device_lerp(tri_b, tri_b + tri_n_b, w);
					const float3 T_c = device_lerp(tri_c, tri_c + tri_n_c, w);


					geoframe.Q[0][0] = (T_b - T_a).x;
					geoframe.Q[1][0] = (T_b - T_a).y;
					geoframe.Q[2][0] = (T_b - T_a).z;

					geoframe.Q[0][1] = (T_c - T_a).x;
					geoframe.Q[1][1] = (T_c - T_a).y;
					geoframe.Q[2][1] = (T_c - T_a).z;

					float3 Ng = ((1.0f - u - v) * tri_n_c + u * tri_n_a + v * tri_n_b);

					geoframe.Q[0][2] = Ng.x;
					geoframe.Q[1][2] = Ng.y;
					geoframe.Q[2][2] = Ng.z;
					//geoframe.QQ[2][2] = geoframe.bbox_max_z - geoframe.bbox_min_z;

					isect->T = geoframe.Q * inverse(geoframe.QQ);
				}

				if (isect->t != FLT_MAX) {
					isect->t = ((isect->t - offset_t) / geoframe.t_mult + (geoframe.t_min));
				}

				/*if (isect->t == FLT_MAX && !has_intersection_in_geopattern && geoframe.isect) {
					isect->type = PRIMITIVE_TRIANGLE;
					isect->u = geoframe.u;
					isect->v = geoframe.v;
					isect->t = geoframe.t;
					isect->prim = geoframe.prim_addr;
					isect->object = geoframe.object;
					isect->entry_object = GEOPATTERN_NO_LINK;
					isect->entry_prim = GEOPATTERN_NO_LINK;
				}*/

				P = geoframe.P;
				dir = geoframe.dir;
				idir = bvh_inverse_direction(dir);
				t_far = geoframe.t_far;
				t_near = geoframe.t_near;

				entry_obj = GEOPATTERN_NO_LINK;
				entry_prim = GEOPATTERN_NO_LINK;
				object = recovered_object;
				node_addr = traversal_stack[stack_ptr];
				--stack_ptr;
#else
				if (geoframe2.rays.current_ray != -1) {
					if (!has_intersection_in_geopattern) {
						if (isect->t != FLT_MAX) {
							float t_mult = geoframe2.rays.t_mult[geoframe2.rays.current_ray];
							float t_min = geoframe2.rays.t_min[geoframe2.rays.current_ray];
							isect->t = (isect->t - offset_t) / t_mult + t_min;
						}
					} else {
						isect->entry_prim = entry_prim;
						isect->entry_object = entry_obj;
						int sd_prim = kernel_tex_fetch(__prim_index, isect->prim);
						const uint4 sd_tri_vindex = kernel_tex_fetch(__tri_vindex, sd_prim);
						const float3 v0 = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, sd_tri_vindex.w+0));
						const float3 v1 = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, sd_tri_vindex.w+1));
						const float3 v2 = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, sd_tri_vindex.w+2));

						//float3 pp = (1.0f - isect->v - isect->u) * v2 + isect->u * v0 + isect->v * v1;

						float3 pp = dir * isect->t + P;

//						float3 v0_n = float4_to_float3(kernel_tex_fetch(__tri_vnormal, sd_tri_vindex.x));
//						float3 v1_n = float4_to_float3(kernel_tex_fetch(__tri_vnormal, sd_tri_vindex.y));
//						float3 v2_n = float4_to_float3(kernel_tex_fetch(__tri_vnormal, sd_tri_vindex.z));


						float3 I = -dir;
						float3 N = triangle_normal(v0, v1, v2, 0);

						if (dot(I, N) < 0.0f) {
							N = -N;
						}

						float3 offset_up = my_ray_offset(pp, N);
						float3 offset_down = my_ray_offset(pp, -N);


						const uint4 tri_vindex = kernel_tex_fetch(__tri_vindex, entry_prim);
						float3 tri_a = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 0));
						float3 tri_b = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 1));
						float3 tri_c = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 2));
						float3 tri_n_a = geoframe2.height *normalize(float4_to_float3(kernel_tex_fetch(__tri_vnormal, tri_vindex.x)));
						float3 tri_n_b = geoframe2.height *normalize(float4_to_float3(kernel_tex_fetch(__tri_vnormal, tri_vindex.y)));
						float3 tri_n_c = geoframe2.height *normalize(float4_to_float3(kernel_tex_fetch(__tri_vnormal, tri_vindex.z)));
						float2 point = make_float2(pp.x, pp.y);
						float u, v;

                        isect->offset_up = inverse_mapping(tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c, geoframe2, offset_up);
                        isect->offset_down = inverse_mapping(tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c, geoframe2, offset_down);

						barycentric_float2(geoframe2.A, geoframe2.B, geoframe2.C, point, u, v);

							float w = clamp((pp.z - geoframe2.bbox_min_z) / (geoframe2.bbox_max_z - geoframe2.bbox_min_z),0.0f, 1.0f);

							//printf("%f %f %f\n", u, v, w);

							const float3 T_a = device_lerp(tri_a, tri_a + tri_n_a, w);
							const float3 T_b = device_lerp(tri_b, tri_b + tri_n_b, w);
							const float3 T_c = device_lerp(tri_c, tri_c + tri_n_c, w);

							float3 worldPoint = ((1.0f - u - v) * T_c + u * T_a + v * T_b);

							float proj_t = fabs(dot(worldPoint - geoframe2.P, geoframe2.dir));

							if (isect->t != FLT_MAX) {
								float t_mult = geoframe2.rays.t_mult[geoframe2.rays.current_ray];
								float t_min = geoframe2.rays.t_min[geoframe2.rays.current_ray];
								isect->t = (isect->t - offset_t) / t_mult + t_min;
							}

							//printf("%f %f diff %f\n",dot(worldPoint - geoframe2.P, geoframe2.dir), t_glob, fabs(dot(worldPoint - geoframe2.P, geoframe2.dir) - t_glob));

							geoframe2.Q[0][0] = (T_b - T_a).x;
							geoframe2.Q[1][0] = (T_b - T_a).y;
							geoframe2.Q[2][0] = (T_b - T_a).z;

							geoframe2.Q[0][1] = (T_c - T_a).x;
							geoframe2.Q[1][1] = (T_c - T_a).y;
							geoframe2.Q[2][1] = (T_c - T_a).z;

							float3 Ng = ((1.0f - u - v) * tri_n_c + u * tri_n_a + v * tri_n_b);

							geoframe2.Q[0][2] = Ng.x;
							geoframe2.Q[1][2] = Ng.y;
							geoframe2.Q[2][2] = Ng.z;
							geoframe2.QQ[2][2] = geoframe2.bbox_max_z - geoframe2.bbox_min_z;
//						} else {
//							printf("FIXME %f %f!!!!\n", u, v);
//							float t_mult = geoframe2.rays.t_mult[geoframe2.rays.current_ray];
//							float t_min = geoframe2.rays.t_min[geoframe2.rays.current_ray];
//							isect->t = (isect->t - offset_t) / t_mult + t_min;
//						}
						isect->T = geoframe2.Q * inverse(geoframe2.QQ);
					}
				}

				++geoframe2.rays.current_ray;
				if (   geoframe2.rays.current_ray < geoframe2.rays.num_rays
				    && isect->t < geoframe2.rays.t_min[geoframe2.rays.current_ray]) {
					geoframe2.rays.current_ray = geoframe2.rays.num_rays;
				}

				if (geoframe2.rays.current_ray < geoframe2.rays.num_rays) {
					++stack_ptr;
					assert(stack_ptr < BVH_STACK_SIZE);
					traversal_stack[stack_ptr] = GEOPATTERN_SENTINEL;

					node_addr = geoframe2.object_nodes;
					object = geoframe2.object;

					P    = geoframe2.rays.P[geoframe2.rays.current_ray];
					dir  = geoframe2.rays.dir[geoframe2.rays.current_ray];
					idir = bvh_inverse_direction(dir);

					t_near = geoframe2.rays.t_near[geoframe2.rays.current_ray];
					t_far  = geoframe2.rays.t_far [geoframe2.rays.current_ray];

					if (isect->t != FLT_MAX) {
                        float t_mult = geoframe2.rays.t_mult[geoframe2.rays.current_ray];
                        float t_min  = geoframe2.rays.t_min [geoframe2.rays.current_ray];
						isect->t = (isect->t - t_min) * t_mult + offset_t;
					}
					has_intersection_in_geopattern = false;
				} else {
					int recovered_object = traversal_stack[stack_ptr];
					--stack_ptr;
					assert(stack_ptr >= 0);

					P = geoframe2.P;
					dir = geoframe2.dir;
					idir = bvh_inverse_direction(dir);
					t_near = geoframe2.t_near;
                    t_far = geoframe2.t_far;

					object = recovered_object;

					entry_obj = GEOPATTERN_NO_LINK;
					entry_prim = GEOPATTERN_NO_LINK;

					node_addr = traversal_stack[stack_ptr];
					--stack_ptr;
				}
#endif
			}

#if BVH_FEATURE(BVH_INSTANCING)
			if (node_addr == INSTANCE_SENTINEL) {
				int recovered_object = traversal_stack[stack_ptr];
				--stack_ptr;
				assert(stack_ptr >= 0);

                kernel_assert(object != OBJECT_NONE);
                /* instance pop */
#  if BVH_FEATURE(BVH_MOTION)
                isect->t = bvh_instance_motion_pop(kg, object, ray, &P, &dir, &idir, isect->t, &ob_itfm);
#  else
                isect->t = bvh_instance_pop(kg, object, ray, &P, &dir, &idir, isect->t);
#  endif
                object = recovered_object;

                node_addr = traversal_stack[stack_ptr];
				--stack_ptr;
            }
#endif  /* FEATURE(BVH_INSTANCING) */

		}
	} while(node_addr != ENTRYPOINT_SENTINEL);

	return (isect->prim != PRIM_NONE);
}

ccl_device_inline bool BVH_FUNCTION_NAME(KernelGlobals *kg,
                                         const Ray *ray,
                                         Intersection *isect,
                                         const uint visibility
#if BVH_FEATURE(BVH_HAIR_MINIMUM_WIDTH)
                                         , uint *lcg_state,
                                         float difl,
                                         float extmax
#endif
                                         )
{
#ifdef __QBVH__
	if(kernel_data.bvh.use_qbvh) {
		return BVH_FUNCTION_FULL_NAME(QBVH)(kg,
		                                    ray,
		                                    isect,
		                                    visibility
#if BVH_FEATURE(BVH_HAIR_MINIMUM_WIDTH)
		                                    , lcg_state,
		                                    difl,
		                                    extmax
#endif
		                                    );
	}
	else
#endif
	{
		kernel_assert(kernel_data.bvh.use_qbvh == false);
		return BVH_FUNCTION_FULL_NAME(BVH)(kg,
		                                   ray,
		                                   isect,
		                                   visibility
#if BVH_FEATURE(BVH_HAIR_MINIMUM_WIDTH)
		                                   , lcg_state,
		                                   difl,
		                                   extmax
#endif
		                                   );
	}
}

#undef BVH_FUNCTION_NAME
#undef BVH_FUNCTION_FEATURES
#undef NODE_INTERSECT
#undef NODE_INTERSECT_ROBUST