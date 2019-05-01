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
#include <util/util_math_float3.h>
#include "bvh_traversal_cuda_types.h"

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
	int object = OBJECT_NONE;

#if BVH_FEATURE(BVH_MOTION)
	Transform ob_itfm;
#endif

	isect->t = ray->t;
	isect->u = 0.0f;
	isect->v = 0.0f;
	isect->prim = PRIM_NONE;
	isect->object = OBJECT_NONE;

	GeopatternFrame geoframe;
	Mat3 T;
	int  entry_prim = GEOPATTERN_NO_LINK;
	int  entry_obj = GEOPATTERN_NO_LINK;
	memset(&geoframe, 0, sizeof(GeopatternFrame));
	float t_far = FLT_MAX;
	float t_near = -FLT_MAX;
    HitData min_hit, max_hit;

	bool debug_enter_prism = false;
    int debug_num_intersect = 0;
	BVH_DEBUG_INIT();

	/* traversal loop */
	do {
		do {
			/* traverse internal nodes */
			while(node_addr >= 0 && node_addr != ENTRYPOINT_SENTINEL) {
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
								BVH_DEBUG_NEXT_INTERSECTION();
								kernel_assert(kernel_tex_fetch(__prim_type, prim_addr) == type);

								int object_idx = ((object == OBJECT_NONE)? kernel_tex_fetch(__prim_object, prim_addr) : object);
								const float4 geopattern_settings = kernel_tex_fetch(__object_geopattern, object_idx);

								const uint geopattern_link = __float_as_uint(geopattern_settings.x);
								const uint geopattern_clipbox_link = __float_as_uint(geopattern_settings.y);
								const float geopatten_height = geopattern_settings.z;
								assert((object == OBJECT_NONE)? true : (geopattern_link == GEOPATTERN_NO_LINK));
								if (geopattern_link == GEOPATTERN_NO_LINK) {
									if (triangle_intersect(kg,
														   isect,
														   P,
														   dir,
														   visibility,
														   object,
														   prim_addr,
														   t_near,
														   t_far)) {
										/* shadow ray early termination */
										isect->entry_object = entry_obj;
										isect->entry_prim = entry_prim;
										isect->T = T;

										if (visibility == PATH_RAY_SHADOW_OPAQUE)
											return true;
									}
								} else {
									const int prim = kernel_tex_fetch(__prim_index, prim_addr);
									const uint4 tri_vindex = kernel_tex_fetch(__tri_vindex, prim);
									int object_flag = kernel_tex_fetch(__object_flag, object_idx);

									float3 tri_a = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 0));
									float3 tri_b = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 1));
									float3 tri_c = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 2));
									float3 tri_n_a = float4_to_float3(kernel_tex_fetch(__tri_vnormal, tri_vindex.x));
									float3 tri_n_b = float4_to_float3(kernel_tex_fetch(__tri_vnormal, tri_vindex.y));
									float3 tri_n_c = float4_to_float3(kernel_tex_fetch(__tri_vnormal, tri_vindex.z));
									tri_n_a *= geopatten_height;
									tri_n_b *= geopatten_height;
									tri_n_c *= geopatten_height;

//									float3 ab = tri_a - tri_b;
//									float3 ac = tri_a - tri_c;
//									float3 shift_a = normalize(ab + ac);
//
//									float3 ba = -ab;
//									float3 bc = tri_b - tri_c;
//									float3 shift_b = normalize(ba + bc);
//
//									float3 ca = -ac;
//									float3 cb = -bc;
//									float3 shift_c = normalize(ca + cb);

//									float3 prism_tri_n_a = (1 + 2 * offset_t) * tri_n_a;
//									float3 prism_tri_n_b = (1 + 2 * offset_t) * tri_n_b;
//									float3 prism_tri_n_c = (1 + 2 * offset_t) * tri_n_c;
//
//									float3 prism_tri_1_a = tri_a + offset_t * shift_a - tri_n_a * offset_t;
//									float3 prism_tri_1_b = tri_b + offset_t * shift_b - tri_n_b * offset_t;
//									float3 prism_tri_1_c = tri_c + offset_t * shift_c - tri_n_c * offset_t;

									float3 &prism_tri_n_a = tri_n_a;
									float3 &prism_tri_n_b = tri_n_b;
									float3 &prism_tri_n_c = tri_n_c;

									float3 &prism_tri_1_a = tri_a;
									float3 &prism_tri_1_b = tri_b;
									float3 &prism_tri_1_c = tri_c;

									float3 prism_tri_2_a = prism_tri_1_a + prism_tri_n_a;
									float3 prism_tri_2_b = prism_tri_1_b + prism_tri_n_b;
									float3 prism_tri_2_c = prism_tri_1_c + prism_tri_n_c;

									float3 prism_tri_3_a = prism_tri_1_a;
									float3 prism_tri_3_b = prism_tri_1_b;
									float3 prism_tri_3_c = prism_tri_1_a + prism_tri_n_a;

									float3 prism_tri_4_a = prism_tri_1_a + prism_tri_n_a;
									float3 prism_tri_4_b = prism_tri_1_b;
									float3 prism_tri_4_c = prism_tri_1_b + prism_tri_n_b;

									float3 prism_tri_5_a = prism_tri_1_b;
									float3 prism_tri_5_b = prism_tri_1_c;
									float3 prism_tri_5_c = prism_tri_1_b + prism_tri_n_b;

									float3 prism_tri_6_a = prism_tri_1_b + prism_tri_n_b;
									float3 prism_tri_6_b = prism_tri_1_c;
									float3 prism_tri_6_c = prism_tri_1_c + prism_tri_n_c;

									float3 prism_tri_7_a = prism_tri_1_c;
									float3 prism_tri_7_b = prism_tri_1_a;
									float3 prism_tri_7_c = prism_tri_1_c + prism_tri_n_c;

									float3 prism_tri_8_a = prism_tri_1_c + prism_tri_n_c;
									float3 prism_tri_8_b = prism_tri_1_a;
									float3 prism_tri_8_c = prism_tri_1_a + prism_tri_n_a;

									min_hit.hit = false;

									min_hit.t = FLT_MAX;
									max_hit.hit = false;

									max_hit.t = -FLT_MAX;
									float u, v, t;

									bool d1 =       (sdistance_to_plane(prism_tri_1_a, prism_tri_1_b, prism_tri_1_c, object_flag, P) > 0.0f);
									bool d2 = d1 && (sdistance_to_plane(prism_tri_2_a, prism_tri_2_b, prism_tri_2_c, object_flag, P) < 0.0f);

									bool d3 = d2 && (sdistance_to_plane(prism_tri_3_a, prism_tri_3_b, prism_tri_3_c, object_flag, P) < 0.0f);
									bool d4 = d3 && (sdistance_to_plane(prism_tri_4_a, prism_tri_4_b, prism_tri_4_c, object_flag, P) < 0.0f);
									bool d5 = d4 && (sdistance_to_plane(prism_tri_5_a, prism_tri_5_b, prism_tri_5_c, object_flag, P) < 0.0f);
									bool d6 = d5 && (sdistance_to_plane(prism_tri_6_a, prism_tri_6_b, prism_tri_6_c, object_flag, P) < 0.0f);
									bool d7 = d6 && (sdistance_to_plane(prism_tri_7_a, prism_tri_7_b, prism_tri_7_c, object_flag, P) < 0.0f);
									bool d8 = d7 && (sdistance_to_plane(prism_tri_8_a, prism_tri_8_b, prism_tri_8_c, object_flag, P) < 0.0f);

									if (d8) {
										min_hit.hit = true;
										min_hit.t = 0;
										min_hit.isUVWSet = false;
										min_hit.p = P;
									}

                                    if (ray_triangle_intersect(P, dir, FLT_MAX, prism_tri_1_a, prism_tri_1_b, prism_tri_1_c, &u, &v, &t)) {
                                        debug_num_intersect++;
										handle_hit(min_hit, max_hit, P, dir, t, u, v, prism_tri_1_a, prism_tri_1_b, prism_tri_1_c);
                                    }

                                    if (ray_triangle_intersect(P,
                                                               dir,
                                                               FLT_MAX,
															   prism_tri_2_a,
															   prism_tri_2_b,
															   prism_tri_2_c,
                                                               &u, &v, &t)) {
                                        debug_num_intersect++;
										handle_hit(min_hit, max_hit, P, dir, t, u, v, prism_tri_2_a, prism_tri_2_b, prism_tri_2_c);
                                    }



									// side a, b

									if (ray_triangle_intersect(P,
															   dir,
															   FLT_MAX,
															   prism_tri_3_a,
															   prism_tri_3_b,
															   prism_tri_3_c,
															   &u, &v, &t)) {
										debug_num_intersect++;
										handle_hit(min_hit, max_hit, P, dir, t, u, v, prism_tri_3_a, prism_tri_3_b, prism_tri_3_c);
									}

									if (ray_triangle_intersect(P,
															   dir,
															   FLT_MAX,
															   prism_tri_4_a,
															   prism_tri_4_b,
															   prism_tri_4_c,
															   &u, &v, &t)) {
										debug_num_intersect++;
										handle_hit(min_hit, max_hit, P, dir, t, u, v, prism_tri_4_a, prism_tri_4_b, prism_tri_4_c);
									}

									// side b, c

									if (ray_triangle_intersect(P,
															   dir,
															   FLT_MAX,
															   prism_tri_5_a,
															   prism_tri_5_b,
															   prism_tri_5_c,
															   &u, &v, &t)) {
										debug_num_intersect++;
										handle_hit(min_hit, max_hit, P, dir, t, u, v, prism_tri_5_a, prism_tri_5_b, prism_tri_5_c);
									}

									if (ray_triangle_intersect(P,
															   dir,
															   FLT_MAX,
															   prism_tri_6_a,
															   prism_tri_6_b,
															   prism_tri_6_c,
															   &u, &v, &t)) {
										debug_num_intersect++;
										handle_hit(min_hit, max_hit, P, dir, t, u, v, prism_tri_6_a, prism_tri_6_b, prism_tri_6_c);

									}
                                    // side c, a

									if (ray_triangle_intersect(P,
															   dir,
															   FLT_MAX,
															   prism_tri_7_a,
															   prism_tri_7_b,
															   prism_tri_7_c,
															   &u, &v, &t)) {
										debug_num_intersect++;
										handle_hit(min_hit, max_hit, P, dir, t, u, v, prism_tri_7_a, prism_tri_7_b, prism_tri_7_c);
									}

									if (ray_triangle_intersect(P,
															   dir,
															   FLT_MAX,
															   prism_tri_8_a,
															   prism_tri_8_b,
															   prism_tri_8_c,
															   &u, &v, &t)) {
										debug_num_intersect++;
										handle_hit(min_hit, max_hit, P, dir, t, u, v, prism_tri_8_a, prism_tri_8_b, prism_tri_8_c);
									}


									if (min_hit.hit && max_hit.hit && (isect->t > min_hit.t) && (min_hit.t < max_hit.t)) {
                                        debug_enter_prism = true;
										if (!min_hit.isUVWSet) {
											min_hit.isUVWSet = true;
											float3 point = min_hit.p;
											float3 uvw = prism_point_uvw_coords(prism_tri_1_a, prism_tri_1_b, prism_tri_1_c, prism_tri_n_a, prism_tri_n_b, prism_tri_n_c, object_flag, point, EPS);

											min_hit.u = uvw.x;
											min_hit.v = uvw.y;
											min_hit.w = uvw.z;
										}

										if (!max_hit.isUVWSet) {
											max_hit.isUVWSet = true;
											float3 point = max_hit.p;
                                            float3 uvw = prism_point_uvw_coords(prism_tri_1_a, prism_tri_1_b, prism_tri_1_c, prism_tri_n_a, prism_tri_n_b, prism_tri_n_c, object_flag, point, EPS);
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

										// Отступы нужны
										float3 bbox_min = {
												fmin(xx.x, xx.y) + shift_eps,
												fmin(yy.x, yy.y) + shift_eps,
												fmin(zz.x, zz.y) - 2 * EPS2
										};

										float3 bbox_max = {
												fmax(xx.z, xx.w) - shift_eps,
												fmax(yy.z, yy.w) - shift_eps,
												fmax(zz.z, zz.w) + 2 * shift_eps
										};

										const float2 tri_a_uv = kernel_tex_fetch(__prim_tri_uv_geopattern, tri_vindex.w + 0);
										const float2 tri_b_uv = kernel_tex_fetch(__prim_tri_uv_geopattern, tri_vindex.w + 1);
										const float2 tri_c_uv = kernel_tex_fetch(__prim_tri_uv_geopattern, tri_vindex.w + 2);
										float3 A_uv = make_float3(device_lerp(bbox_min.x, bbox_max.x, tri_a_uv.x),
																  device_lerp(bbox_min.y, bbox_max.y, tri_a_uv.y),
																  device_lerp(bbox_min.z, bbox_max.z, min_hit.w));

										float3 B_uv = make_float3(device_lerp(bbox_min.x, bbox_max.x, tri_b_uv.x),
																  device_lerp(bbox_min.y, bbox_max.y, tri_b_uv.y),
																  device_lerp(bbox_min.z, bbox_max.z, min_hit.w));

										float3 C_uv = make_float3(device_lerp(bbox_min.x, bbox_max.x, tri_c_uv.x),
																  device_lerp(bbox_min.y, bbox_max.y, tri_c_uv.y),
																  device_lerp(bbox_min.z, bbox_max.z, min_hit.w));

										float3 A2_uv = make_float3(device_lerp(bbox_min.x, bbox_max.x, tri_a_uv.x),
																   device_lerp(bbox_min.y, bbox_max.y, tri_a_uv.y),
																   device_lerp(bbox_min.z, bbox_max.z, max_hit.w));

										float3 B2_uv = make_float3(device_lerp(bbox_min.x, bbox_max.x, tri_b_uv.x),
																   device_lerp(bbox_min.y, bbox_max.y, tri_b_uv.y),
																   device_lerp(bbox_min.z, bbox_max.z, max_hit.w));

										float3 C2_uv = make_float3(device_lerp(bbox_min.x, bbox_max.x, tri_c_uv.x),
																   device_lerp(bbox_min.y, bbox_max.y, tri_c_uv.y),
																   device_lerp(bbox_min.z, bbox_max.z, max_hit.w));

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


//                                        Transform tfm = object_fetch_transform(kg, object, OBJECT_TRANSFORM);

                                        //printf("%f %f %f\n%f %f %f\n%f %f %f\n\n", tfm.x.x, tfm.x.y, tfm.x.z, tfm.y.x, tfm.y.y, tfm.y.z, tfm.z.x, tfm.z.y, tfm.z.z);

                                        QQ[0][0] = (B_uv - A_uv).x;
                                        QQ[1][0] = (B_uv - A_uv).y;
                                        QQ[2][0] = (B_uv - A_uv).z;

                                        QQ[0][1] = (C_uv - A_uv).x;
                                        QQ[1][1] = (C_uv - A_uv).y;
                                        QQ[2][1] = (C_uv - A_uv).z;

                                        float3 N = triangle_normal(A_uv, B_uv, C_uv, 0);
                                        if ((bbox_max.z - bbox_min.z) > 1.0) {
                                            N *= (bbox_max.z - bbox_min.z);
                                        }

										QQ[0][2] = N.x;
										QQ[1][2] = N.y;
										QQ[2][2] = N.z;

										T = Q * inverse(QQ);

                                        float3 start_texture  = (1.0f - min_hit.u - min_hit.v) * C_uv  + min_hit.u * A_uv  + min_hit.v * B_uv;
                                        float3 end_texture    = (1.0f - max_hit.u - max_hit.v) * C2_uv + max_hit.u * A2_uv + max_hit.v * B2_uv;
										float3 new_dir = bvh_clamp_direction(normalize(end_texture - start_texture));

                                        start_texture -= offset_t * new_dir;
										end_texture += offset_t * new_dir;
                                        /*float2 start_uv =
												(1.0f - min_hit.u - min_hit.v) * tri_c_uv + min_hit.u * tri_a_uv + min_hit.v * tri_b_uv;
										float2 end_uv =
												(1.0f - max_hit.u - max_hit.v) * tri_c_uv + max_hit.u * tri_a_uv + max_hit.v * tri_b_uv;*/

										//printf("%f %f\n", min_hit.t, max_hit.t);

										/*float3 uvw_origin = make_float3(start_uv.x, start_uv.y, min_hit.w);
										float3 uvw_end    = make_float3(end_uv.x, end_uv.y, max_hit.w);*/

										/*float3 start_texture = make_float3(device_lerp(bbox_min.x, bbox_max.x, uvw_origin.x),
																	       device_lerp(bbox_min.y, bbox_max.y, uvw_origin.y),
																	       device_lerp(bbox_min.z, bbox_max.z, uvw_origin.z));
										float3 end_texture = make_float3(device_lerp(bbox_min.x, bbox_max.x, uvw_end.x),
																         device_lerp(bbox_min.y, bbox_max.y, uvw_end.y),
																         device_lerp(bbox_min.z, bbox_max.z, uvw_end.z));*/
										geoframe.dir = dir;
										geoframe.P = P;
										geoframe.t_near = t_near;
										geoframe.t_far = t_far;
										float len_texture = len(end_texture - start_texture);
										float len_world = max_hit.t - min_hit.t;
										float mult = (len_texture / len_world);

										if (isect->t != FLT_MAX) {
											isect->t = (isect->t - min_hit.t) * mult + offset_t;
										}

										geoframe.min_t = min_hit.t;
										geoframe.mult_t = mult;
										t_far = len_texture - offset_t;
										t_near = offset_t;

										dir = new_dir;
										idir = bvh_inverse_direction(dir);
										P = start_texture;

										entry_obj = object_idx;
										entry_prim = prim;
									}
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
					                               			  t_near
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

                    ++stack_ptr;
                    kernel_assert(stack_ptr < BVH_STACK_SIZE);
                    traversal_stack[stack_ptr] = ENTRYPOINT_SENTINEL;

					node_addr = kernel_tex_fetch(__object_node, object);

					BVH_DEBUG_NEXT_INSTANCE();
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
				if (isect->t != FLT_MAX) {
					isect->t = (isect->t - offset_t) / geoframe.mult_t + min_hit.t;
				}

//                if (isect->t == FLT_MAX) {
//                    const uint4 tri_vindex = kernel_tex_fetch(__tri_vindex, entry_prim);
//                    float3 tri_a = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 0));
//                    float3 tri_b = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 1));
//                    float3 tri_c = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 2));
//
//                    float u, v, t;
//                    if (ray_triangle_intersect(geoframe.P,
//                                               geoframe.dir,
//                                               FLT_MAX,
//                                               tri_a,
//                                               tri_b,
//                                               tri_c,
//                                               &u, &v, &t)) {
//
//                        int node_addr = kernel_tex_fetch(__object_node, object);
//                        assert(node_addr >= 0);
//                        float4 xx = kernel_tex_fetch(__bvh_nodes, node_addr + 1);
//                        float4 yy = kernel_tex_fetch(__bvh_nodes, node_addr + 2);
//                        float4 zz = kernel_tex_fetch(__bvh_nodes, node_addr + 3);
//
//                        float3 bbox_min = {
//                                min(xx.x, xx.y),
//                                min(yy.x, yy.y),
//                                min(zz.x, zz.y)
//                        };
//
//                        float3 bbox_max = {
//                                max(xx.z, xx.w),
//                                max(yy.z, yy.w),
//                                max(zz.z, zz.w)
//                        };
//
//                        const float2 tri_a_uv = kernel_tex_fetch(__prim_tri_uv_geopattern, tri_vindex.w + 0);
//                        const float2 tri_b_uv = kernel_tex_fetch(__prim_tri_uv_geopattern, tri_vindex.w + 1);
//                        const float2 tri_c_uv = kernel_tex_fetch(__prim_tri_uv_geopattern, tri_vindex.w + 2);
//
//                        float3 _A_uv = make_float3(device_lerp(bbox_min.x + shift_eps, bbox_max.x - shift_eps, tri_a_uv.x),
//                                                   device_lerp(bbox_min.y + shift_eps, bbox_max.y - shift_eps, tri_a_uv.y),
//                                                   bbox_min.z);
//
//                        float3 _B_uv = make_float3(device_lerp(bbox_min.x + shift_eps, bbox_max.x - shift_eps, tri_b_uv.x),
//                                                   device_lerp(bbox_min.y + shift_eps, bbox_max.y - shift_eps, tri_b_uv.y),
//                                                   bbox_min.z);
//
//                        float3 _C_uv = make_float3(device_lerp(bbox_min.x + shift_eps, bbox_max.x - shift_eps, tri_c_uv.x),
//                                                   device_lerp(bbox_min.y + shift_eps, bbox_max.y - shift_eps, tri_c_uv.y),
//                                                   bbox_min.z);
//
//                        float u_uv, v_uv, t_uv = FLT_MAX;
//
//
//						float3 A_uv = make_float3(device_lerp(bbox_min.x + shift_eps, bbox_max.x - shift_eps, tri_a_uv.x),
//												  device_lerp(bbox_min.y + shift_eps, bbox_max.y - shift_eps, tri_a_uv.y),
//												  device_lerp(bbox_min.z, bbox_max.z + 4* shift_eps, min_hit.w));
//
//						float3 B_uv = make_float3(device_lerp(bbox_min.x + shift_eps, bbox_max.x - shift_eps, tri_b_uv.x),
//												  device_lerp(bbox_min.y + shift_eps, bbox_max.y - shift_eps, tri_b_uv.y),
//												  device_lerp(bbox_min.z, bbox_max.z + 4 * shift_eps, min_hit.w));
//
//						float3 C_uv = make_float3(device_lerp(bbox_min.x + shift_eps, bbox_max.x - shift_eps, tri_c_uv.x),
//												  device_lerp(bbox_min.y + shift_eps, bbox_max.y - shift_eps, tri_c_uv.y),
//												  device_lerp(bbox_min.z, bbox_max.z + 4 * shift_eps, min_hit.w));
//
//						float3 A2_uv = make_float3(device_lerp(bbox_min.x + shift_eps, bbox_max.x - shift_eps, tri_a_uv.x),
//												   device_lerp(bbox_min.y + shift_eps, bbox_max.y - shift_eps, tri_a_uv.y),
//												   device_lerp(bbox_min.z, bbox_max.z + 4 * shift_eps, max_hit.w));
//
//						float3 B2_uv = make_float3(device_lerp(bbox_min.x + shift_eps, bbox_max.x - shift_eps, tri_b_uv.x),
//												   device_lerp(bbox_min.y + shift_eps, bbox_max.y - shift_eps, tri_b_uv.y),
//												   device_lerp(bbox_min.z, bbox_max.z + 4 * shift_eps, max_hit.w));
//
//						float3 C2_uv = make_float3(device_lerp(bbox_min.x + shift_eps, bbox_max.x - shift_eps, tri_c_uv.x),
//												   device_lerp(bbox_min.y + shift_eps, bbox_max.y - shift_eps, tri_c_uv.y),
//												   device_lerp(bbox_min.z, bbox_max.z + 4 * shift_eps, max_hit.w));
//
//                        float3 start_texture  = (1.0f - min_hit.u - min_hit.v) * C_uv  + min_hit.u * A_uv  + min_hit.v * B_uv;
//                        float3 end_texture    = (1.0f - max_hit.u - max_hit.v) * C2_uv + max_hit.u * A2_uv + max_hit.v * B2_uv;
//
//                        P -= 5 * dir;
//
//                        if (ray_triangle_intersect(P, dir,
//                                                   FLT_MAX,
//												   A_uv,
//												   B_uv,
//												   C_uv,
//                                                   &u_uv, &v_uv, &t_uv)) {}
//
//                        printf("P %f %f %f | dir %f %f %f debug_enter_prism %d\n"
//                               "entry_P%f %f %f\n"
//                               "exit_P%f %f %f\n"
//                               "%f t to bottom\n"
//                               "%f t_min\n"
//                               "%f t_max\n"
//                               "%f t_uv\n"
//                               "debug_num_intersect %d\n"
//                               //"%f %f %f | %f %f %f | %f %f %f\n"
//                               "sdist = P %f t_s %f t_e %f\n"
//                               "bbox min %f %f %f\n"
//                               "bbox max %f %f %f\n"
//                               "%f\n"
//                               "\n\n",
//                               P.x, P.y, P.z,
//                               dir.x, dir.y, dir.z,
//                               debug_enter_prism,
//                               start_texture.x, start_texture.y, start_texture.z,
//                               end_texture.x, end_texture.y, end_texture.z,
//                               t, min_hit.t, max_hit.t, t_uv,
//                               debug_num_intersect,
//                              // _A_uv.x, _A_uv.y, _A_uv.z, _B_uv.x, _B_uv.y, _B_uv.z, _C_uv.x, _C_uv.y, _C_uv.z,
//                               sdistance_to_plane(_A_uv, _B_uv, _C_uv, 0, P),
//                               sdistance_to_plane(_A_uv, _B_uv, _C_uv, 0, start_texture),
//                               sdistance_to_plane(_A_uv, _B_uv, _C_uv, 0, end_texture),
//                               bbox_min.x, bbox_min.y, bbox_min.z,
//                               bbox_max.x, bbox_max.y, bbox_max.z,
//                               0.0f);
//                    }
//                }
                P = geoframe.P;
                dir = geoframe.dir;
                idir = bvh_inverse_direction(dir);
                t_far = geoframe.t_far;
				t_near = geoframe.t_near;

                debug_num_intersect = 0;
                debug_enter_prism = false;

				entry_obj = GEOPATTERN_NO_LINK;
				entry_prim = GEOPATTERN_NO_LINK;
			}
#if BVH_FEATURE(BVH_INSTANCING)
			if (sentinel == INSTANCE_SENTINEL) {
                kernel_assert(object != OBJECT_NONE);
                /* instance pop */
#  if BVH_FEATURE(BVH_MOTION)
                isect->t = bvh_instance_motion_pop(kg, object, ray, &P, &dir, &idir, isect->t, &ob_itfm);
#  else
                isect->t = bvh_instance_pop(kg, object, ray, &P, &dir, &idir, isect->t);
#  endif
            }
#endif  /* FEATURE(BVH_INSTANCING) */

			object = recovered_object;
			node_addr = traversal_stack[stack_ptr];
			--stack_ptr;
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


///*
// * Adapted from code Copyright 2009-2010 NVIDIA Corporation,
// * and code copyright 2009-2012 Intel Corporation
// *
// * Modifications Copyright 2011-2013, Blender Foundation.
// *
// * Licensed under the Apache License, Version 2.0 (the "License");
// * you may not use this file except in compliance with the License.
// * You may obtain a copy of the License at
// *
// * http://www.apache.org/licenses/LICENSE-2.0
// *
// * Unless required by applicable law or agreed to in writing, software
// * distributed under the License is distributed on an "AS IS" BASIS,
// * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// * See the License for the specific language governing permissions and
// * limitations under the License.
// */
//#ifdef __QBVH__
//#  include "kernel/bvh/qbvh_traversal.h"
//#endif
//
//#if BVH_FEATURE(BVH_HAIR)
//#  define NODE_INTERSECT bvh_node_intersect
//#  define NODE_INTERSECT_ROBUST bvh_node_intersect_robust
//#else
//#  define NODE_INTERSECT bvh_aligned_node_intersect
//#  define NODE_INTERSECT_ROBUST bvh_aligned_node_intersect_robust
//#endif
//
///* This is a template BVH traversal function, where various features can be
// * enabled/disabled. This way we can compile optimized versions for each case
// * without new features slowing things down.
// *
// * BVH_INSTANCING: object instancing
// * BVH_HAIR: hair curve rendering
// * BVH_HAIR_MINIMUM_WIDTH: hair curve rendering with minimum width
// * BVH_MOTION: motion blur rendering
// *
// */
//#include <kernel/kernel_types.h>
//#include <util/util_math_float3.h>
//#include "bvh_traversal_cuda_types.h"
//
//ccl_device_noinline bool BVH_FUNCTION_FULL_NAME(BVH)(KernelGlobals *kg,
//													 const Ray *ray,
//													 Intersection *isect,
//													 const uint visibility
//#if BVH_FEATURE(BVH_HAIR_MINIMUM_WIDTH)
//, uint *lcg_state,
//                                                     float difl,
//                                                     float extmax
//#endif
//)
//{
//	/* todo:
//	 * - test if pushing distance on the stack helps (for non shadow rays)
//	 * - separate version for shadow rays
//	 * - likely and unlikely for if() statements
//	 * - test restrict attribute for pointers
//	 */
//
//	/* traversal stack in CUDA thread-local memory */
//	int traversal_stack[BVH_STACK_SIZE];
//	traversal_stack[0] = ENTRYPOINT_SENTINEL;
//
//	/* traversal variables in registers */
//	int stack_ptr = 0;
//	int node_addr = kernel_data.bvh.root;
//
//	/* ray parameters in registers */
//	float3 P = ray->P;
//	float3 dir = bvh_clamp_direction(ray->D);
//	float3 idir = bvh_inverse_direction(dir);
//	int object = OBJECT_NONE;
//
//#if BVH_FEATURE(BVH_MOTION)
//	Transform ob_itfm;
//#endif
//
//	isect->t = ray->t;
//	isect->u = 0.0f;
//	isect->v = 0.0f;
//	isect->prim = PRIM_NONE;
//	isect->object = OBJECT_NONE;
//
//	GeopatternFrame geoframe;
//	Mat3 T;
//	int  entry_prim = GEOPATTERN_NO_LINK;
//	int  entry_obj = GEOPATTERN_NO_LINK;
//	memset(&geoframe, 0, sizeof(GeopatternFrame));
//	float t_far = FLT_MAX;
//	float t_near = -FLT_MAX;
//	HitData min_hit, max_hit;
//
//	bool debug_enter_prism = false;
//	int debug_num_intersect = 0;
//	BVH_DEBUG_INIT();
//
//	/* traversal loop */
//	do {
//		do {
//			/* traverse internal nodes */
//			while(node_addr >= 0 && node_addr != ENTRYPOINT_SENTINEL) {
//				int node_addr_child1, traverse_mask;
//				float dist[2];
//				float4 cnodes = kernel_tex_fetch(__bvh_nodes, node_addr+0);
//
//#  if BVH_FEATURE(BVH_HAIR_MINIMUM_WIDTH)
//				if(difl != 0.0f) {
//					traverse_mask = NODE_INTERSECT_ROBUST(kg,
//					                                      P,
//#    if BVH_FEATURE(BVH_HAIR)
//					                                      dir,
//#    endif
//					                                      idir,
//					                                      isect->t,
//					                                      difl,
//					                                      extmax,
//					                                      node_addr,
//					                                      visibility,
//					                                      dist,
//					                                      t_near,
//					                                      t_far);
//				}
//				else
//#  endif
//				{
//					traverse_mask = NODE_INTERSECT(kg,
//												   P,
//#    if BVH_FEATURE(BVH_HAIR)
//							dir,
//#    endif
//												   idir,
//												   isect->t,
//												   node_addr,
//												   visibility,
//												   dist,
//												   t_near,
//												   t_far);
//				}
//
//				node_addr = __float_as_int(cnodes.z);
//				node_addr_child1 = __float_as_int(cnodes.w);
//
//				if(traverse_mask == 3) {
//					/* Both children were intersected, push the farther one. */
//					bool is_closest_child1 = (dist[1] < dist[0]);
//					if(is_closest_child1) {
//						int tmp = node_addr;
//						node_addr = node_addr_child1;
//						node_addr_child1 = tmp;
//					}
//
//					++stack_ptr;
//					kernel_assert(stack_ptr < BVH_STACK_SIZE);
//					traversal_stack[stack_ptr] = node_addr_child1;
//				}
//				else {
//					/* One child was intersected. */
//					if(traverse_mask == 2) {
//						node_addr = node_addr_child1;
//					}
//					else if(traverse_mask == 0) {
//						/* Neither child was intersected. */
//						node_addr = traversal_stack[stack_ptr];
//						--stack_ptr;
//					}
//				}
//				BVH_DEBUG_NEXT_NODE();
//			}
//
//			/* if node is leaf, fetch triangle list */
//			if(node_addr < 0) {
//				float4 leaf = kernel_tex_fetch(__bvh_leaf_nodes, (-node_addr-1));
//				int prim_addr = __float_as_int(leaf.x);
//
//#if BVH_FEATURE(BVH_INSTANCING)
//				if(prim_addr >= 0) {
//#endif
//				const int prim_addr2 = __float_as_int(leaf.y);
//				const uint type = __float_as_int(leaf.w);
//
//				/* pop */
//				node_addr = traversal_stack[stack_ptr];
//				--stack_ptr;
//
//				/* primitive intersection */
//				switch(type & PRIMITIVE_ALL) {
//					case PRIMITIVE_TRIANGLE: {
//						for(; prim_addr < prim_addr2; prim_addr++) {
//							BVH_DEBUG_NEXT_INTERSECTION();
//							kernel_assert(kernel_tex_fetch(__prim_type, prim_addr) == type);
//
//							int object_idx = ((object == OBJECT_NONE)? kernel_tex_fetch(__prim_object, prim_addr) : object);
//							const float4 geopattern_settings = kernel_tex_fetch(__object_geopattern, object_idx);
//
//							const uint geopattern_link = __float_as_uint(geopattern_settings.x);
//							const uint geopattern_clipbox_link = __float_as_uint(geopattern_settings.y);
//							const float geopatten_height = geopattern_settings.z;
//							assert((object == OBJECT_NONE)? true : (geopattern_link == GEOPATTERN_NO_LINK));
//							if (geopattern_link == GEOPATTERN_NO_LINK) {
//								if (triangle_intersect(kg,
//													   isect,
//													   P,
//													   dir,
//													   visibility,
//													   object,
//													   prim_addr,
//													   t_near,
//													   t_far)) {
//									/* shadow ray early termination */
//									isect->entry_object = entry_obj;
//									isect->entry_prim = entry_prim;
//									isect->T = T;
//
//									if (visibility == PATH_RAY_SHADOW_OPAQUE)
//										return true;
//								}
//							} else {
//								const int prim = kernel_tex_fetch(__prim_index, prim_addr);
//								const uint4 tri_vindex = kernel_tex_fetch(__tri_vindex, prim);
//								int object_flag = kernel_tex_fetch(__object_flag, object_idx);
//
//								float3 tri_a = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 0));
//								float3 tri_b = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 1));
//								float3 tri_c = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 2));
//								float3 tri_n_a = float4_to_float3(kernel_tex_fetch(__tri_vnormal, tri_vindex.x));
//								float3 tri_n_b = float4_to_float3(kernel_tex_fetch(__tri_vnormal, tri_vindex.y));
//								float3 tri_n_c = float4_to_float3(kernel_tex_fetch(__tri_vnormal, tri_vindex.z));
//
//								tri_n_a *= geopatten_height;
//								tri_n_b *= geopatten_height;
//								tri_n_c *= geopatten_height;
//
//								min_hit.hit = false;
//
//								min_hit.t = FLT_MAX;
//								max_hit.hit = false;
//
//								max_hit.t = -FLT_MAX;
//								float u, v, t;
//
//								bool d1 =       (sdistance_to_plane(tri_a,           tri_b,           tri_c          , object_flag, P) > 0.0f);
//								bool d2 = d1 && (sdistance_to_plane(tri_a + tri_n_a, tri_b + tri_n_b, tri_c + tri_n_c, object_flag, P) < 0.0f);
//
//								bool d3 = d2 && (sdistance_to_plane(tri_a,           tri_b,           tri_a + tri_n_a, object_flag, P) < 0.0f);
//								bool d4 = d3 && (sdistance_to_plane(tri_a + tri_n_a, tri_b          , tri_b + tri_n_b, object_flag, P) < 0.0f);
//
//								bool d5 = d4 && (sdistance_to_plane(tri_b,           tri_c,           tri_b + tri_n_b, object_flag, P) < 0.0f);
//								bool d6 = d5 && (sdistance_to_plane(tri_b + tri_n_b, tri_c          , tri_c + tri_n_c, object_flag, P) < 0.0f);
//
//								bool d7 = d6 && (sdistance_to_plane(tri_c,           tri_a,           tri_c + tri_n_c, object_flag, P) < 0.0f);
//								bool d8 = d7 && (sdistance_to_plane(tri_c + tri_n_c, tri_a          , tri_a + tri_n_a, object_flag, P) < 0.0f);
//
//								if (d8) {
//									min_hit.hit = true;
//									min_hit.t = 0;
//									min_hit.isUVWSet = false;
//									min_hit.p = P;
//								}
//
//								if (ray_triangle_intersect(P,
//														   dir,
//														   FLT_MAX,
//														   tri_a + tri_n_a,
//														   tri_b + tri_n_b,
//														   tri_c + tri_n_c,
//														   &u, &v, &t)) {
//									debug_num_intersect++;
//									handle_hit(min_hit, max_hit, P, dir, t, u, v, tri_a + tri_n_a, tri_b + tri_n_b, tri_c + tri_n_c);
//								}
//
//								if (ray_triangle_intersect(P,
//														   dir,
//														   FLT_MAX,
//														   tri_a,
//														   tri_b,
//														   tri_c,
//														   &u, &v, &t)) {
//									debug_num_intersect++;
//									handle_hit(min_hit, max_hit, P, dir, t, u, v, tri_a, tri_b, tri_c);
//								}
//
//
//
//								// side a, b
//								{
//									{
//										float3 a = tri_a;
//										float3 b = tri_b;
//										float3 c = tri_a + tri_n_a;
//										if (ray_triangle_intersect(P,
//																   dir,
//																   FLT_MAX,
//																   a,
//																   b,
//																   c,
//																   &u, &v, &t)) {
//											debug_num_intersect++;
//											handle_hit(min_hit, max_hit, P, dir, t, u, v, a, b, c);
//										}
//									}
//
//									{
//										float3 a = tri_a + tri_n_a;
//										float3 b = tri_b;
//										float3 c = tri_b + tri_n_b;
//										if (ray_triangle_intersect(P,
//																   dir,
//																   FLT_MAX,
//																   a,
//																   b,
//																   c,
//																   &u, &v, &t)) {
//											debug_num_intersect++;
//											handle_hit(min_hit, max_hit, P, dir, t, u, v, a, b, c);
//										}
//									}
//								}
//
//								// side b, c
//								{
//									{
//										float3 a = tri_b;
//										float3 b = tri_c;
//										float3 c = tri_b + tri_n_b;
//										if (ray_triangle_intersect(P,
//																   dir,
//																   FLT_MAX,
//																   a,
//																   b,
//																   c,
//																   &u, &v, &t)) {
//											debug_num_intersect++;
//											handle_hit(min_hit, max_hit, P, dir, t, u, v, a, b, c);
//										}
//									}
//									{
//										float3 a = tri_b + tri_n_b;
//										float3 b = tri_c;
//										float3 c = tri_c + tri_n_c;
//										if (ray_triangle_intersect(P,
//																   dir,
//																   FLT_MAX,
//																   a,
//																   b,
//																   c,
//																   &u, &v, &t)) {
//											debug_num_intersect++;
//											handle_hit(min_hit, max_hit, P, dir, t, u, v, a, b, c);
//										}
//									}
//								}
//								// side c, a
//								{
//									{
//										float3 a = tri_c;
//										float3 b = tri_a;
//										float3 c = tri_c + tri_n_c;
//										if (ray_triangle_intersect(P,
//																   dir,
//																   FLT_MAX,
//																   a,
//																   b,
//																   c,
//																   &u, &v, &t)) {
//											debug_num_intersect++;
//											handle_hit(min_hit, max_hit, P, dir, t, u, v, a, b, c);
//										}
//									}
//									{
//										float3 a = tri_c + tri_n_c;
//										float3 b = tri_a;
//										float3 c = tri_a + tri_n_a;
//										if (ray_triangle_intersect(P,
//																   dir,
//																   FLT_MAX,
//																   a,
//																   b,
//																   c,
//																   &u, &v, &t)) {
//											debug_num_intersect++;
//											handle_hit(min_hit, max_hit, P, dir, t, u, v, a, b, c);
//										}
//									}
//								}
//
//								if (min_hit.hit && max_hit.hit && (isect->t > min_hit.t) && (min_hit.t < max_hit.t)) {
//									debug_enter_prism = true;
//									if (!min_hit.isUVWSet) {
//										min_hit.isUVWSet = true;
//										float3 point = min_hit.p;
//										float3 uvw = prism_point_uvw_coords(tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c, object_flag,
//																			point, EPS);
//
//										min_hit.u = uvw.x;
//										min_hit.v = uvw.y;
//										min_hit.w = uvw.z;
//									}
//
//									if (!max_hit.isUVWSet) {
//										max_hit.isUVWSet = true;
//										float3 point = max_hit.p;
//										float3 uvw = prism_point_uvw_coords(tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c, object_flag,
//																			point, EPS);
//										max_hit.u = uvw.x;
//										max_hit.v = uvw.y;
//										max_hit.w = uvw.z;
//									}
//
//									++stack_ptr;
//									assert(stack_ptr < BVH_STACK_SIZE);
//									traversal_stack[stack_ptr] = node_addr;
//
//									++stack_ptr;
//									assert(stack_ptr < BVH_STACK_SIZE);
//									traversal_stack[stack_ptr] = object;
//
//									++stack_ptr;
//									assert(stack_ptr < BVH_STACK_SIZE);
//									traversal_stack[stack_ptr] = GEOPATTERN_SENTINEL;
//
//									++stack_ptr;
//									assert(stack_ptr < BVH_STACK_SIZE);
//									traversal_stack[stack_ptr] = ENTRYPOINT_SENTINEL;
//
//									object = geopattern_link;
//
//									node_addr = kernel_tex_fetch(__object_node, object);
//									assert(node_addr >= 0);
//									float4 xx = kernel_tex_fetch(__bvh_nodes, node_addr + 1);
//									float4 yy = kernel_tex_fetch(__bvh_nodes, node_addr + 2);
//									float4 zz = kernel_tex_fetch(__bvh_nodes, node_addr + 3);
//
//									float3 bbox_min = {
//											fmin(xx.x + EPS2, xx.y + EPS2),
//											fmin(yy.x + EPS2, yy.y + EPS2),
//											fmin(zz.x, zz.y) - EPS2
//									};
//
//									float3 bbox_max = {
//											fmax(xx.z - EPS2, xx.w - EPS2),
//											fmax(yy.z - EPS2, yy.w - EPS2),
//											fmax(zz.z, zz.w) + EPS2
//									};
//
//									const float2 tri_a_uv = kernel_tex_fetch(__prim_tri_uv_geopattern, tri_vindex.w + 0);
//									const float2 tri_b_uv = kernel_tex_fetch(__prim_tri_uv_geopattern, tri_vindex.w + 1);
//									const float2 tri_c_uv = kernel_tex_fetch(__prim_tri_uv_geopattern, tri_vindex.w + 2);
//									// Отступы нужны
//									float3 A_uv = make_float3(device_lerp(bbox_min.x, bbox_max.x, tri_a_uv.x),
//															  device_lerp(bbox_min.y, bbox_max.y, tri_a_uv.y),
//															  device_lerp(bbox_min.z, bbox_max.z, min_hit.w));
//
//									float3 B_uv = make_float3(device_lerp(bbox_min.x, bbox_max.x, tri_b_uv.x),
//															  device_lerp(bbox_min.y, bbox_max.y, tri_b_uv.y),
//															  device_lerp(bbox_min.z, bbox_max.z, min_hit.w));
//
//									float3 C_uv = make_float3(device_lerp(bbox_min.x, bbox_max.x, tri_c_uv.x),
//															  device_lerp(bbox_min.y, bbox_max.y, tri_c_uv.y),
//															  device_lerp(bbox_min.z, bbox_max.z, min_hit.w));
//
//									float3 A2_uv = make_float3(device_lerp(bbox_min.x, bbox_max.x, tri_a_uv.x),
//															   device_lerp(bbox_min.y, bbox_max.y, tri_a_uv.y),
//															   device_lerp(bbox_min.z, bbox_max.z, max_hit.w));
//
//									float3 B2_uv = make_float3(device_lerp(bbox_min.x, bbox_max.x, tri_b_uv.x),
//															   device_lerp(bbox_min.y, bbox_max.y, tri_b_uv.y),
//															   device_lerp(bbox_min.z, bbox_max.z, max_hit.w));
//
//									float3 C2_uv = make_float3(device_lerp(bbox_min.x, bbox_max.x, tri_c_uv.x),
//															   device_lerp(bbox_min.y, bbox_max.y, tri_c_uv.y),
//															   device_lerp(bbox_min.z, bbox_max.z, max_hit.w));
//
//									Mat3 Q;
//									Mat3 QQ;
//
//									if (object_flag & D_SD_OBJECT_NEGATIVE_SCALE_APPLIED) {
//										Q[0][0] = (tri_c - tri_a).x;
//										Q[1][0] = (tri_c - tri_a).y;
//										Q[2][0] = (tri_c - tri_a).z;
//
//										Q[0][1] = (tri_b - tri_a).x;
//										Q[1][1] = (tri_b - tri_a).y;
//										Q[2][1] = (tri_b - tri_a).z;
//
//									} else {
//										Q[0][0] = (tri_b - tri_a).x;
//										Q[1][0] = (tri_b - tri_a).y;
//										Q[2][0] = (tri_b - tri_a).z;
//
//										Q[0][1] = (tri_c - tri_a).x;
//										Q[1][1] = (tri_c - tri_a).y;
//										Q[2][1] = (tri_c - tri_a).z;
//									}
//
//									float3 Ng = geopatten_height * triangle_normal(tri_a, tri_b, tri_c, object_flag);
//
//
//									Q[0][2] = Ng.x;
//									Q[1][2] = Ng.y;
//									Q[2][2] = Ng.z;
//
//
////                                        Transform tfm = object_fetch_transform(kg, object, OBJECT_TRANSFORM);
//
//									//printf("%f %f %f\n%f %f %f\n%f %f %f\n\n", tfm.x.x, tfm.x.y, tfm.x.z, tfm.y.x, tfm.y.y, tfm.y.z, tfm.z.x, tfm.z.y, tfm.z.z);
//
//									QQ[0][0] = (B_uv - A_uv).x;
//									QQ[1][0] = (B_uv - A_uv).y;
//									QQ[2][0] = (B_uv - A_uv).z;
//
//									QQ[0][1] = (C_uv - A_uv).x;
//									QQ[1][1] = (C_uv - A_uv).y;
//									QQ[2][1] = (C_uv - A_uv).z;
//
//									float3 N = triangle_normal(A_uv, B_uv, C_uv, 0);
//									if ((bbox_max.z - bbox_min.z + 2 * shift_eps) > 1.0) {
//										N *= (bbox_max.z - bbox_min.z + 2 * shift_eps);
//									}
//
//									QQ[0][2] = N.x;
//									QQ[1][2] = N.y;
//									QQ[2][2] = N.z;
//
//									T = Q * inverse(QQ);
//
//									float3 start_texture  = (1.0f - min_hit.u - min_hit.v) * C_uv  + min_hit.u * A_uv  + min_hit.v * B_uv;
//									float3 end_texture    = (1.0f - max_hit.u - max_hit.v) * C2_uv + max_hit.u * A2_uv + max_hit.v * B2_uv;
//
//									float3 new_dir = bvh_clamp_direction(normalize(end_texture - start_texture));
//									float len_texture = len(end_texture - start_texture);
//									start_texture -= offset_t * new_dir;
//
//									/*float2 start_uv =
//                                            (1.0f - min_hit.u - min_hit.v) * tri_c_uv + min_hit.u * tri_a_uv + min_hit.v * tri_b_uv;
//                                    float2 end_uv =
//                                            (1.0f - max_hit.u - max_hit.v) * tri_c_uv + max_hit.u * tri_a_uv + max_hit.v * tri_b_uv;*/
//
//									//printf("%f %f\n", min_hit.t, max_hit.t);
//
//									/*float3 uvw_origin = make_float3(start_uv.x, start_uv.y, min_hit.w);
//                                    float3 uvw_end    = make_float3(end_uv.x, end_uv.y, max_hit.w);*/
//
//									/*float3 start_texture = make_float3(device_lerp(bbox_min.x, bbox_max.x, uvw_origin.x),
//                                                                       device_lerp(bbox_min.y, bbox_max.y, uvw_origin.y),
//                                                                       device_lerp(bbox_min.z, bbox_max.z, uvw_origin.z));
//                                    float3 end_texture = make_float3(device_lerp(bbox_min.x, bbox_max.x, uvw_end.x),
//                                                                     device_lerp(bbox_min.y, bbox_max.y, uvw_end.y),
//                                                                     device_lerp(bbox_min.z, bbox_max.z, uvw_end.z));*/
//									geoframe.dir = dir;
//									geoframe.P = P;
//									geoframe.t_far = t_far;
//									geoframe.t_near = t_near;
//
//									float len_world = max_hit.t;
//									float mult = (len_texture / len_world);
//
//									if (isect->t != FLT_MAX) {
//										isect->t = (isect->t * mult) + offset_t;
//									}
//
//									geoframe.mult_t = mult;
//
//									t_far = len_texture;
//									t_near = offset_t;
//									dir = new_dir;
//									idir = bvh_inverse_direction(dir);
//									P = start_texture;
//
//									entry_obj = object_idx;
//									entry_prim = prim;
//								}
//							}
//						}
//						break;
//					}
//#if BVH_FEATURE(BVH_MOTION)
//					case PRIMITIVE_MOTION_TRIANGLE: {
//							for(; prim_addr < prim_addr2; prim_addr++) {
//								BVH_DEBUG_NEXT_INTERSECTION();
//								kernel_assert(kernel_tex_fetch(__prim_type, prim_addr) == type);
//								if(motion_triangle_intersect(kg,
//								                             isect,
//								                             P,
//								                             dir,
//								                             ray->time,
//								                             visibility,
//								                             object,
//								                             prim_addr,
//								                             t_near,
//								                             t_far))
//								{
//									isect->entry_object = entry_obj;
//									isect->entry_prim = entry_prim;
//									/* shadow ray early termination */
//									if(visibility == PATH_RAY_SHADOW_OPAQUE)
//										return true;
//								}
//							}
//							break;
//						}
//#endif  /* BVH_FEATURE(BVH_MOTION) */
//#if BVH_FEATURE(BVH_HAIR)
//					case PRIMITIVE_CURVE:
//						case PRIMITIVE_MOTION_CURVE: {
//							for(; prim_addr < prim_addr2; prim_addr++) {
//								BVH_DEBUG_NEXT_INTERSECTION();
//								const uint curve_type = kernel_tex_fetch(__prim_type, prim_addr);
//								kernel_assert((curve_type & PRIMITIVE_ALL) == (type & PRIMITIVE_ALL));
//								bool hit;
//								if(kernel_data.curve.curveflags & CURVE_KN_INTERPOLATE) {
//									hit = bvh_cardinal_curve_intersect(kg,
//									                                   isect,
//									                                   P,
//									                                   dir,
//									                                   visibility,
//									                                   object,
//									                                   prim_addr,
//									                                   ray->time,
//									                                   curve_type,
//									                                   lcg_state,
//									                                   difl,
//									                                   extmax,
//									                                   t_near,
//									                                   t_far);
//								}
//								else {
//									hit = bvh_curve_intersect(kg,
//									                          isect,
//									                          P,
//									                          dir,
//									                          visibility,
//									                          object,
//									                          prim_addr,
//									                          ray->time,
//									                          curve_type,
//									                          lcg_state,
//									                          difl,
//									                          extmax,
//					                               			  t_near
//					                               			  t_far);
//								}
//								if(hit) {
//									/* shadow ray early termination */
//									if(visibility == PATH_RAY_SHADOW_OPAQUE)
//										return true;
//								}
//							}
//							break;
//						}
//#endif  /* BVH_FEATURE(BVH_HAIR) */
//				}
//			}
//#if BVH_FEATURE(BVH_INSTANCING)
//			else {
//					/* instance push */
//					int prev_object = object;
//					object = kernel_tex_fetch(__prim_object, -prim_addr-1);
//
//#  if BVH_FEATURE(BVH_MOTION)
//					isect->t = bvh_instance_motion_push(kg, object, ray, &P, &dir, &idir, isect->t, &ob_itfm);
//#  else
//					isect->t = bvh_instance_push(kg, object, ray, &P, &dir, &idir, isect->t);
//#  endif
//					 ++stack_ptr;
//                    kernel_assert(stack_ptr < BVH_STACK_SIZE);
//                    traversal_stack[stack_ptr] = prev_object;
//
//					++stack_ptr;
//                    assert(stack_ptr < BVH_STACK_SIZE);
//                    traversal_stack[stack_ptr] = INSTANCE_SENTINEL;
//
//                    ++stack_ptr;
//                    kernel_assert(stack_ptr < BVH_STACK_SIZE);
//                    traversal_stack[stack_ptr] = ENTRYPOINT_SENTINEL;
//
//					node_addr = kernel_tex_fetch(__object_node, object);
//
//					BVH_DEBUG_NEXT_INSTANCE();
//				}
//			}
//#endif  /* FEATURE(BVH_INSTANCING) */
//		} while(node_addr != ENTRYPOINT_SENTINEL);
//		if (stack_ptr > 0) {
//			int sentinel = traversal_stack[stack_ptr];
//			--stack_ptr;
//			assert(stack_ptr >= 0);
//			int recovered_object = traversal_stack[stack_ptr];
//			--stack_ptr;
//			assert(stack_ptr >= 0);
//
//			if (sentinel == GEOPATTERN_SENTINEL) {
//				if (isect->t != FLT_MAX) {
//					isect->t = (isect->t - offset_t) / geoframe.mult_t;
//				}
//
////                if (isect->t == FLT_MAX) {
////                    const uint4 tri_vindex = kernel_tex_fetch(__tri_vindex, entry_prim);
////                    float3 tri_a = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 0));
////                    float3 tri_b = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 1));
////                    float3 tri_c = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 2));
////
////                    float u, v, t;
////                    if (ray_triangle_intersect(geoframe.P,
////                                               geoframe.dir,
////                                               FLT_MAX,
////                                               tri_a,
////                                               tri_b,
////                                               tri_c,
////                                               &u, &v, &t)) {
////
////                        int node_addr = kernel_tex_fetch(__object_node, object);
////                        assert(node_addr >= 0);
////                        float4 xx = kernel_tex_fetch(__bvh_nodes, node_addr + 1);
////                        float4 yy = kernel_tex_fetch(__bvh_nodes, node_addr + 2);
////                        float4 zz = kernel_tex_fetch(__bvh_nodes, node_addr + 3);
////
////                        float3 bbox_min = {
////                                min(xx.x, xx.y),
////                                min(yy.x, yy.y),
////                                min(zz.x, zz.y)
////                        };
////
////                        float3 bbox_max = {
////                                max(xx.z, xx.w),
////                                max(yy.z, yy.w),
////                                max(zz.z, zz.w)
////                        };
////
////                        const float2 tri_a_uv = kernel_tex_fetch(__prim_tri_uv_geopattern, tri_vindex.w + 0);
////                        const float2 tri_b_uv = kernel_tex_fetch(__prim_tri_uv_geopattern, tri_vindex.w + 1);
////                        const float2 tri_c_uv = kernel_tex_fetch(__prim_tri_uv_geopattern, tri_vindex.w + 2);
////
////                        float3 _A_uv = make_float3(device_lerp(bbox_min.x + shift_eps, bbox_max.x - shift_eps, tri_a_uv.x),
////                                                   device_lerp(bbox_min.y + shift_eps, bbox_max.y - shift_eps, tri_a_uv.y),
////                                                   bbox_min.z);
////
////                        float3 _B_uv = make_float3(device_lerp(bbox_min.x + shift_eps, bbox_max.x - shift_eps, tri_b_uv.x),
////                                                   device_lerp(bbox_min.y + shift_eps, bbox_max.y - shift_eps, tri_b_uv.y),
////                                                   bbox_min.z);
////
////                        float3 _C_uv = make_float3(device_lerp(bbox_min.x + shift_eps, bbox_max.x - shift_eps, tri_c_uv.x),
////                                                   device_lerp(bbox_min.y + shift_eps, bbox_max.y - shift_eps, tri_c_uv.y),
////                                                   bbox_min.z);
////
////                        float u_uv, v_uv, t_uv = FLT_MAX;
////
////
////						float3 A_uv = make_float3(device_lerp(bbox_min.x + shift_eps, bbox_max.x - shift_eps, tri_a_uv.x),
////												  device_lerp(bbox_min.y + shift_eps, bbox_max.y - shift_eps, tri_a_uv.y),
////												  device_lerp(bbox_min.z, bbox_max.z + 4* shift_eps, min_hit.w));
////
////						float3 B_uv = make_float3(device_lerp(bbox_min.x + shift_eps, bbox_max.x - shift_eps, tri_b_uv.x),
////												  device_lerp(bbox_min.y + shift_eps, bbox_max.y - shift_eps, tri_b_uv.y),
////												  device_lerp(bbox_min.z, bbox_max.z + 4 * shift_eps, min_hit.w));
////
////						float3 C_uv = make_float3(device_lerp(bbox_min.x + shift_eps, bbox_max.x - shift_eps, tri_c_uv.x),
////												  device_lerp(bbox_min.y + shift_eps, bbox_max.y - shift_eps, tri_c_uv.y),
////												  device_lerp(bbox_min.z, bbox_max.z + 4 * shift_eps, min_hit.w));
////
////						float3 A2_uv = make_float3(device_lerp(bbox_min.x + shift_eps, bbox_max.x - shift_eps, tri_a_uv.x),
////												   device_lerp(bbox_min.y + shift_eps, bbox_max.y - shift_eps, tri_a_uv.y),
////												   device_lerp(bbox_min.z, bbox_max.z + 4 * shift_eps, max_hit.w));
////
////						float3 B2_uv = make_float3(device_lerp(bbox_min.x + shift_eps, bbox_max.x - shift_eps, tri_b_uv.x),
////												   device_lerp(bbox_min.y + shift_eps, bbox_max.y - shift_eps, tri_b_uv.y),
////												   device_lerp(bbox_min.z, bbox_max.z + 4 * shift_eps, max_hit.w));
////
////						float3 C2_uv = make_float3(device_lerp(bbox_min.x + shift_eps, bbox_max.x - shift_eps, tri_c_uv.x),
////												   device_lerp(bbox_min.y + shift_eps, bbox_max.y - shift_eps, tri_c_uv.y),
////												   device_lerp(bbox_min.z, bbox_max.z + 4 * shift_eps, max_hit.w));
////
////                        float3 start_texture  = (1.0f - min_hit.u - min_hit.v) * C_uv  + min_hit.u * A_uv  + min_hit.v * B_uv;
////                        float3 end_texture    = (1.0f - max_hit.u - max_hit.v) * C2_uv + max_hit.u * A2_uv + max_hit.v * B2_uv;
////
////                        P -= 5 * dir;
////
////                        if (ray_triangle_intersect(P, dir,
////                                                   FLT_MAX,
////												   A_uv,
////												   B_uv,
////												   C_uv,
////                                                   &u_uv, &v_uv, &t_uv)) {}
////
////                        printf("P %f %f %f | dir %f %f %f debug_enter_prism %d\n"
////                               "entry_P%f %f %f\n"
////                               "exit_P%f %f %f\n"
////                               "%f t to bottom\n"
////                               "%f t_min\n"
////                               "%f t_max\n"
////                               "%f t_uv\n"
////                               "debug_num_intersect %d\n"
////                               //"%f %f %f | %f %f %f | %f %f %f\n"
////                               "sdist = P %f t_s %f t_e %f\n"
////                               "bbox min %f %f %f\n"
////                               "bbox max %f %f %f\n"
////                               "%f\n"
////                               "\n\n",
////                               P.x, P.y, P.z,
////                               dir.x, dir.y, dir.z,
////                               debug_enter_prism,
////                               start_texture.x, start_texture.y, start_texture.z,
////                               end_texture.x, end_texture.y, end_texture.z,
////                               t, min_hit.t, max_hit.t, t_uv,
////                               debug_num_intersect,
////                              // _A_uv.x, _A_uv.y, _A_uv.z, _B_uv.x, _B_uv.y, _B_uv.z, _C_uv.x, _C_uv.y, _C_uv.z,
////                               sdistance_to_plane(_A_uv, _B_uv, _C_uv, 0, P),
////                               sdistance_to_plane(_A_uv, _B_uv, _C_uv, 0, start_texture),
////                               sdistance_to_plane(_A_uv, _B_uv, _C_uv, 0, end_texture),
////                               bbox_min.x, bbox_min.y, bbox_min.z,
////                               bbox_max.x, bbox_max.y, bbox_max.z,
////                               0.0f);
////                    }
////                }
//				P = geoframe.P;
//				dir = geoframe.dir;
//				idir = bvh_inverse_direction(dir);
//				t_far = geoframe.t_far;
//				t_near = geoframe.t_near;
//
//				debug_num_intersect = 0;
//				debug_enter_prism = false;
//
//				entry_obj = GEOPATTERN_NO_LINK;
//				entry_prim = GEOPATTERN_NO_LINK;
//			}
//#if BVH_FEATURE(BVH_INSTANCING)
//			if (sentinel == INSTANCE_SENTINEL) {
//                kernel_assert(object != OBJECT_NONE);
//                /* instance pop */
//#  if BVH_FEATURE(BVH_MOTION)
//                isect->t = bvh_instance_motion_pop(kg, object, ray, &P, &dir, &idir, isect->t, &ob_itfm);
//#  else
//                isect->t = bvh_instance_pop(kg, object, ray, &P, &dir, &idir, isect->t);
//#  endif
//            }
//#endif  /* FEATURE(BVH_INSTANCING) */
//
//			object = recovered_object;
//			node_addr = traversal_stack[stack_ptr];
//			--stack_ptr;
//		}
//	} while(node_addr != ENTRYPOINT_SENTINEL);
//
//	return (isect->prim != PRIM_NONE);
//}
//
//ccl_device_inline bool BVH_FUNCTION_NAME(KernelGlobals *kg,
//										 const Ray *ray,
//										 Intersection *isect,
//										 const uint visibility
//#if BVH_FEATURE(BVH_HAIR_MINIMUM_WIDTH)
//, uint *lcg_state,
//                                         float difl,
//                                         float extmax
//#endif
//)
//{
//#ifdef __QBVH__
//	if(kernel_data.bvh.use_qbvh) {
//		return BVH_FUNCTION_FULL_NAME(QBVH)(kg,
//											ray,
//											isect,
//											visibility
//#if BVH_FEATURE(BVH_HAIR_MINIMUM_WIDTH)
//		, lcg_state,
//		                                    difl,
//		                                    extmax
//#endif
//		);
//	}
//	else
//#endif
//	{
//		kernel_assert(kernel_data.bvh.use_qbvh == false);
//		return BVH_FUNCTION_FULL_NAME(BVH)(kg,
//										   ray,
//										   isect,
//										   visibility
//#if BVH_FEATURE(BVH_HAIR_MINIMUM_WIDTH)
//		, lcg_state,
//		                                   difl,
//		                                   extmax
//#endif
//		);
//	}
//}
//
//#undef BVH_FUNCTION_NAME
//#undef BVH_FUNCTION_FEATURES
//#undef NODE_INTERSECT
//#undef NODE_INTERSECT_ROBUST
