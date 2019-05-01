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
	Mat3 T;
	int  entry_prim = GEOPATTERN_NO_LINK;
	int  entry_obj = GEOPATTERN_NO_LINK;
	float isect_t = tmax;
	float t_lim = tmax;
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
											   t_lim);

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
                                int object_idx = ((object == OBJECT_NONE)? kernel_tex_fetch(__prim_object, prim_addr) : object);
								const float4 geopattern_settings = kernel_tex_fetch(__object_geopattern, object_idx);
								const uint geopattern_link = __float_as_uint(geopattern_settings.x);
								const uint geopattern_clipbox_link = __float_as_uint(geopattern_settings.y);
								const float geopatten_height = geopattern_settings.z;
								assert((object == OBJECT_NONE)? true : (geopattern_link == GEOPATTERN_NO_LINK));
								if (geopattern_link == GEOPATTERN_NO_LINK) {
									hit = triangle_intersect(kg,
															 isect_array,
															 P,
															 dir,
															 PATH_RAY_SHADOW,
															 object,
															 prim_addr,
															 t_lim);

									if (hit) {
										isect_array->entry_object = entry_obj;
										isect_array->entry_prim = entry_prim;
										isect_array->T = T;
									}

								} else {
									const int prim = kernel_tex_fetch(__prim_index, prim_addr);
									const uint4 tri_vindex = kernel_tex_fetch(__tri_vindex, prim);
                                    int object_flag = kernel_tex_fetch(__object_flag, object_idx);

									const float3 tri_a = float4_to_float3(
											kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 0));
									const float3 tri_b = float4_to_float3(
											kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 1));
									const float3 tri_c = float4_to_float3(
											kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 2));

									float3 tri_n_a = float4_to_float3(kernel_tex_fetch(__tri_vnormal, tri_vindex.x));
									float3 tri_n_b = float4_to_float3(kernel_tex_fetch(__tri_vnormal, tri_vindex.y));
									float3 tri_n_c = float4_to_float3(kernel_tex_fetch(__tri_vnormal, tri_vindex.z));

									tri_n_a *= geopatten_height;
									tri_n_b *= geopatten_height;
									tri_n_c *= geopatten_height;

									HitData min_hit, max_hit;
									min_hit.hit = false;
									min_hit.t = FLT_MAX;

									max_hit.hit = false;
									max_hit.t = -FLT_MAX;
									float u, v, t;

                                    bool d1 =       (sdistance_to_plane(tri_a,           tri_b,           tri_c          , object_flag, P) > 0.0f);
                                    bool d2 = d1 && (sdistance_to_plane(tri_a + tri_n_a, tri_b + tri_n_b, tri_c + tri_n_c, object_flag, P) < 0.0f);

                                    bool d3 = d2 && (sdistance_to_plane(tri_a,           tri_b,           tri_a + tri_n_a, object_flag, P) < 0.0f);
                                    bool d4 = d3 && (sdistance_to_plane(tri_a + tri_n_a, tri_b          , tri_b + tri_n_b, object_flag, P) < 0.0f);

                                    bool d5 = d4 && (sdistance_to_plane(tri_b,           tri_c,           tri_b + tri_n_b, object_flag, P) < 0.0f);
                                    bool d6 = d5 && (sdistance_to_plane(tri_b + tri_n_b, tri_c          , tri_c + tri_n_c, object_flag, P) < 0.0f);

                                    bool d7 = d6 && (sdistance_to_plane(tri_c,           tri_a,           tri_c + tri_n_c, object_flag, P) < 0.0f);
                                    bool d8 = d7 && (sdistance_to_plane(tri_c + tri_n_c, tri_a          , tri_a + tri_n_a, object_flag, P) < 0.0f);

                                    if (d8) {
                                        min_hit.hit = true;
                                        min_hit.t = 0;
                                        min_hit.isUVWSet = false;
                                        min_hit.p = P;
                                    }


                                    if (ray_triangle_intersect(P,
                                                               dir,
                                                               FLT_MAX,
                                                               tri_a + tri_n_a,
                                                               tri_b + tri_n_b,
                                                               tri_c + tri_n_c,
                                                               &u, &v, &t)) {
                                        handle_hit(min_hit, max_hit, P, dir, t, u, v, tri_a + tri_n_a, tri_b + tri_n_b, tri_c + tri_n_c);
                                    }

                                    if (ray_triangle_intersect(P,
                                                               dir,
                                                               FLT_MAX,
                                                               tri_a,
                                                               tri_b,
                                                               tri_c,
                                                               &u, &v, &t)) {
                                        handle_hit(min_hit, max_hit, P, dir, t, u, v, tri_a, tri_b, tri_c);
                                    }


                                    // side a, b
                                    {
                                        {
                                            float3 a = tri_a;
                                            float3 b = tri_b;
                                            float3 c = tri_a + tri_n_a;
                                            if (ray_triangle_intersect(P,
                                                                       dir,
                                                                       FLT_MAX,
                                                                       a,
                                                                       b,
                                                                       c,
                                                                       &u, &v, &t)) {
                                                handle_hit(min_hit, max_hit, P, dir, t, u, v, a, b, c);
                                            }
                                        }

                                        {
                                            float3 a = tri_a + tri_n_a;
                                            float3 b = tri_b;
                                            float3 c = tri_b + tri_n_b;
                                            if (ray_triangle_intersect(P,
                                                                       dir,
                                                                       FLT_MAX,
                                                                       a,
                                                                       b,
                                                                       c,
                                                                       &u, &v, &t)) {
                                                handle_hit(min_hit, max_hit, P, dir, t, u, v, a, b, c);
                                            }
                                        }
                                    }

                                    // side b, c
                                    {
                                        {
                                            float3 a = tri_b;
                                            float3 b = tri_c;
                                            float3 c = tri_b + tri_n_b;
                                            if (ray_triangle_intersect(P,
                                                                       dir,
                                                                       FLT_MAX,
                                                                       a,
                                                                       b,
                                                                       c,
                                                                       &u, &v, &t)) {
                                                handle_hit(min_hit, max_hit, P, dir, t, u, v, a, b, c);
                                            }
                                        }
                                        {
                                            float3 a = tri_b + tri_n_b;
                                            float3 b = tri_c;
                                            float3 c = tri_c + tri_n_c;
                                            if (ray_triangle_intersect(P,
                                                                       dir,
                                                                       FLT_MAX,
                                                                       a,
                                                                       b,
                                                                       c,
                                                                       &u, &v, &t)) {
                                                handle_hit(min_hit, max_hit, P, dir, t, u, v, a, b, c);
                                            }
                                        }
                                    }
                                    // side c, a
                                    {
                                        {
                                            float3 a = tri_c;
                                            float3 b = tri_a;
                                            float3 c = tri_c + tri_n_c;
                                            if (ray_triangle_intersect(P,
                                                                       dir,
                                                                       FLT_MAX,
                                                                       a,
                                                                       b,
                                                                       c,
                                                                       &u, &v, &t)) {
                                                handle_hit(min_hit, max_hit, P, dir, t, u, v, a, b, c);
                                            }
                                        }
                                        {
                                            float3 a = tri_c + tri_n_c;
                                            float3 b = tri_a;
                                            float3 c = tri_a + tri_n_a;
                                            if (ray_triangle_intersect(P,
                                                                       dir,
                                                                       FLT_MAX,
                                                                       a,
                                                                       b,
                                                                       c,
                                                                       &u, &v, &t)) {
                                                handle_hit(min_hit, max_hit, P, dir, t, u, v, a, b, c);
                                            }
                                        }
                                    }

                                    if (min_hit.hit && max_hit.hit && (isect_array->t > min_hit.t) && (min_hit.t < max_hit.t)) {
                                        if (!min_hit.isUVWSet) {
                                            min_hit.isUVWSet = true;
                                            float3 point = min_hit.p;
                                            float3 uvw = prism_point_uvw_coords(tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c, object_flag,
                                                                                point, EPS);
                                            min_hit.u = uvw.x;
                                            min_hit.v = uvw.y;
                                            min_hit.w = uvw.z;
                                        }

                                        if (!max_hit.isUVWSet) {
                                            max_hit.isUVWSet = true;
                                            float3 point = max_hit.p;
                                            float3 uvw = prism_point_uvw_coords(tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c, object_flag,
                                                                                point, EPS);
                                            max_hit.u = uvw.x;
                                            max_hit.v = uvw.y;
                                            max_hit.w = uvw.z;
                                        }

										++stack_ptr;
										assert(stack_ptr < BVH_STACK_SIZE);
										traversal_stack[stack_ptr] = node_addr;

										++stack_ptr;
										assert(stack_ptr < BVH_STACK_SIZE);
										traversal_stack[stack_ptr] = object;

										++stack_ptr;
										assert(stack_ptr < BVH_STACK_SIZE);
										traversal_stack[stack_ptr] = GEOPATTERN_SENTINEL;

										++stack_ptr;
										assert(stack_ptr < BVH_STACK_SIZE);
										traversal_stack[stack_ptr] = ENTRYPOINT_SENTINEL;

										object = geopattern_link;

										node_addr = kernel_tex_fetch(__object_node, object);
										assert(node_addr >= 0);
										float4 xx = kernel_tex_fetch(__bvh_nodes, node_addr + 1);
										float4 yy = kernel_tex_fetch(__bvh_nodes, node_addr + 2);
										float4 zz = kernel_tex_fetch(__bvh_nodes, node_addr + 3);

										float3 bbox_min = {
												fmin(xx.x, xx.y),
												fmin(yy.x, yy.y),
												fmin(zz.x, zz.y)
										};

										float3 bbox_max = {
												fmax(xx.z, xx.w),
												fmax(yy.z, yy.w),
												fmax(zz.z, zz.w)
										};


										/*float2 start_uv =
												(1.0f - min_hit.u - min_hit.v) * tri_c_uv + min_hit.u * tri_a_uv + min_hit.v * tri_b_uv;
										float2 end_uv =
												(1.0f - max_hit.u - max_hit.v) * tri_c_uv + max_hit.u * tri_a_uv + max_hit.v * tri_b_uv;

										float3 uvw_origin = make_float3(start_uv.x, start_uv.y, min_hit.w);
										float3 uvw_end = make_float3(end_uv.x, end_uv.y, max_hit.w);


										float3 start_texture = make_float3(
												device_lerp(bbox_min.x, bbox_max.x, uvw_origin.x),
												device_lerp(bbox_min.y, bbox_max.y, uvw_origin.y),
												device_lerp(bbox_min.z, bbox_max.z, uvw_origin.z));
										float3 end_texture = make_float3(device_lerp(bbox_min.x, bbox_max.x, uvw_end.x),
																		 device_lerp(bbox_min.y, bbox_max.y, uvw_end.y),
																		 device_lerp(bbox_min.z, bbox_max.z,
																					 uvw_end.z));*/

										const float2 tri_a_uv = kernel_tex_fetch(__prim_tri_uv_geopattern, tri_vindex.w + 0);
										const float2 tri_b_uv = kernel_tex_fetch(__prim_tri_uv_geopattern, tri_vindex.w + 1);
										const float2 tri_c_uv = kernel_tex_fetch(__prim_tri_uv_geopattern, tri_vindex.w + 2);

                                        float3 A_uv = make_float3(device_lerp(bbox_min.x + shift_eps, bbox_max.x - shift_eps, tri_a_uv.x),
                                                                  device_lerp(bbox_min.y + shift_eps, bbox_max.y - shift_eps, tri_a_uv.y),
                                                                  device_lerp(bbox_min.z - shift_eps, bbox_max.z + shift_eps, min_hit.w));

                                        float3 B_uv = make_float3(device_lerp(bbox_min.x + shift_eps, bbox_max.x - shift_eps, tri_b_uv.x),
                                                                  device_lerp(bbox_min.y + shift_eps, bbox_max.y - shift_eps, tri_b_uv.y),
                                                                  device_lerp(bbox_min.z - shift_eps, bbox_max.z + shift_eps, min_hit.w));

                                        float3 C_uv = make_float3(device_lerp(bbox_min.x + shift_eps, bbox_max.x - shift_eps, tri_c_uv.x),
                                                                  device_lerp(bbox_min.y + shift_eps, bbox_max.y - shift_eps, tri_c_uv.y),
                                                                  device_lerp(bbox_min.z - shift_eps, bbox_max.z + shift_eps, min_hit.w));

                                        float3 A2_uv = make_float3(device_lerp(bbox_min.x + shift_eps, bbox_max.x - shift_eps, tri_a_uv.x),
                                                                   device_lerp(bbox_min.y + shift_eps, bbox_max.y - shift_eps, tri_a_uv.y),
                                                                   device_lerp(bbox_min.z - shift_eps, bbox_max.z + shift_eps, max_hit.w));

                                        float3 B2_uv = make_float3(device_lerp(bbox_min.x + shift_eps, bbox_max.x - shift_eps, tri_b_uv.x),
                                                                   device_lerp(bbox_min.y + shift_eps, bbox_max.y - shift_eps, tri_b_uv.y),
                                                                   device_lerp(bbox_min.z - shift_eps, bbox_max.z + shift_eps, max_hit.w));

                                        float3 C2_uv = make_float3(device_lerp(bbox_min.x + shift_eps, bbox_max.x - shift_eps, tri_c_uv.x),
                                                                   device_lerp(bbox_min.y + shift_eps, bbox_max.y - shift_eps, tri_c_uv.y),
                                                                   device_lerp(bbox_min.z - shift_eps, bbox_max.z + shift_eps, max_hit.w));

                                        Mat3 Q;
                                        Mat3 QQ;

                                        if (object_flag & D_SD_OBJECT_NEGATIVE_SCALE_APPLIED) {
                                            Q[0][0] = (tri_c - tri_a).x;
                                            Q[1][0] = (tri_c - tri_a).y;
                                            Q[2][0] = (tri_c - tri_a).z;

                                            Q[0][1] = (tri_b - tri_a).x;
                                            Q[1][1] = (tri_b - tri_a).y;
                                            Q[2][1] = (tri_b - tri_a).z;

                                        } else {
                                            Q[0][0] = (tri_b - tri_a).x;
                                            Q[1][0] = (tri_b - tri_a).y;
                                            Q[2][0] = (tri_b - tri_a).z;

                                            Q[0][1] = (tri_c - tri_a).x;
                                            Q[1][1] = (tri_c - tri_a).y;
                                            Q[2][1] = (tri_c - tri_a).z;
                                        }

                                        float3 Ng = geopatten_height * triangle_normal(tri_a, tri_b, tri_c, object_flag);

                                        Q[0][2] = Ng.x;
                                        Q[1][2] = Ng.y;
                                        Q[2][2] = Ng.z;
                                        QQ[0][0] = (B_uv - A_uv).x;
                                        QQ[1][0] = (B_uv - A_uv).y;
                                        QQ[2][0] = (B_uv - A_uv).z;

                                        QQ[0][1] = (C_uv - A_uv).x;
                                        QQ[1][1] = (C_uv - A_uv).y;
                                        QQ[2][1] = (C_uv - A_uv).z;

                                        float3 N = triangle_normal(A_uv, B_uv, C_uv, 0);
                                        if ((bbox_max.z - bbox_min.z + 2 * shift_eps) > 1.0) {
                                            N *= (bbox_max.z - bbox_min.z + 2 * shift_eps);
                                        }

                                        QQ[0][2] = N.x;
                                        QQ[1][2] = N.y;
                                        QQ[2][2] = N.z;

                                        T = Q * inverse(QQ);

										float3 start_texture  = (1.0f - min_hit.u - min_hit.v) * C_uv  + min_hit.u * A_uv  + min_hit.v * B_uv;
										float3 end_texture    = (1.0f - max_hit.u - max_hit.v) * C2_uv + max_hit.u * A2_uv + max_hit.v * B2_uv;
																				geoframe.t_lim = t_lim;
										float len_texture = len(end_texture - start_texture);

                                        float len_world = len(max_hit.p - min_hit.p);
                                        float mult = (len_texture / len_world);
                                        if (isect_t != FLT_MAX) {
											isect_t = (isect_t - min_hit.t) * mult;
                                        }
                                        geoframe.min_t = min_hit.t;
                                        geoframe.mult_t = mult;

										t_lim = len_texture;
										geoframe.dir = dir;
										geoframe.P = P;
										dir = bvh_clamp_direction(normalize(end_texture - start_texture));
										idir = bvh_inverse_direction(dir);
										P = start_texture;

										num_hits_in_geopattern = 0;
										isect_array->t = isect_t;
										entry_obj = object_idx;
										entry_prim = prim;
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
								                                t_lim);
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
									                                   t_lim);
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
									                          t_lim);
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

                    ++stack_ptr;
                    kernel_assert(stack_ptr < BVH_STACK_SIZE);
                    traversal_stack[stack_ptr] = ENTRYPOINT_SENTINEL;

					node_addr = kernel_tex_fetch(__object_node, object);
				}
			}
#endif  /* FEATURE(BVH_INSTANCING) */
		} while(node_addr != ENTRYPOINT_SENTINEL);
		if (stack_ptr > 0) {
			int sentinel = traversal_stack[stack_ptr];
			--stack_ptr;
			assert(stack_ptr >= 0);
			int recovered_object = traversal_stack[stack_ptr];
			--stack_ptr;
			assert(stack_ptr >= 0);
			if (sentinel == GEOPATTERN_SENTINEL) {
				if(num_hits_in_geopattern) {
					for(int i = 0; i < num_hits_in_geopattern; i++) {
						(isect_array-i-1)->t /= geoframe.mult_t;
						(isect_array-i-1)->t += geoframe.min_t;
					}
				}
				P = geoframe.P;
				dir = geoframe.dir;
				idir = bvh_inverse_direction(dir);
				t_lim = geoframe.t_lim;
				isect_t = tmax;
				isect_array->t = isect_t;

				entry_obj = GEOPATTERN_NO_LINK;
				entry_prim = GEOPATTERN_NO_LINK;
			}
#if BVH_FEATURE(BVH_INSTANCING)
			if (sentinel == INSTANCE_SENTINEL) {
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
            }
#endif  /* FEATURE(BVH_INSTANCING) */
			object = recovered_object;
			node_addr = traversal_stack[stack_ptr];
			--stack_ptr;
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
    return false;
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
