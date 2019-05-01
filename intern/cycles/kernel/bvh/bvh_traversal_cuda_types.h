#pragma once

#define GEOPATTERN_SENTINEL_RAY (ENTRYPOINT_SENTINEL + 1)
#define GEOPATTERN_SENTINEL (ENTRYPOINT_SENTINEL + 2)
#define INSTANCE_SENTINEL  (ENTRYPOINT_SENTINEL + 3)
#define _MAX_SENTINEL (ENTRYPOINT_SENTINEL + 4)

#define IS_SENTINEL(x) ((x) >= ENTRYPOINT_SENTINEL && (x) < _MAX_SENTINEL)

#include "util/util_mat.h"
#include "../kernel_types.h"
#define  D_SD_OBJECT_NEGATIVE_SCALE_APPLIED (1 << 3)

const float EPS = 1e-5f;
const float EPS2 = 1e-4f;
const float shift_eps = 1e-3f;
const float offset_t = 1e-4f;

enum class Side {
    Top,
    Down,
    AB,
    BC,
    CA
};

enum class Edge {
    AB,
    BC,
    CA
};


struct HitData {
    float t;
    bool hit;
    bool isUVWSet = false;
    //union {
    float3 p;
    float u, v, w;
    //};
};

struct HitData_2 {
    float t;
    bool hit = false;
    float3 p;
    float u, v, w;
};

struct HitData_3 {
    float t;
    float3 p;
    float u, v, w;
    Side side;
};

struct GeopatternFrame {
//    bool second_enter = false;
    float3 dir;
    float3 P;
    float t_near;
    float t_far;
    float t_mult;
    float t_min;
//    int entry_prim;
//    int num_rays;
//    float u[4];
//    float v[4];
//    float w[4];
//    float t[4];
//    Mat3 T[4];
};

struct GeopatternRayFrame {
    bool second_enter = false;
    float3 dir;
    float3 P;
    float t_near;
    float t_far;
    float t_min;
    float t_mult;
    Mat3 T;
};

#define PUSH_STRUCT_ON_STACK(type, ptr, stack, stack_pos) { \
    constexpr size_t size = ((sizeof(type) + (sizeof(int) - 1)) / sizeof(int)); \
    stack_pos++; \
    ptr = ((type *)&stack[stack_pos]); \
    stack_pos += size; \
}

#define POP_STRUCT_FROM_STACK(type, ptr, stack, stack_pos) { \
    constexpr size_t size = ((sizeof(type) + (sizeof(int) - 1)) / sizeof(int)); \
    stack_pos =  stack_pos - size + 1; \
    ptr = ((type *)&stack[stack_pos]); \
    --stack_pos; \
}

ccl_device_inline void barycentric(const float3 tri_a, const float3 tri_b, const float3 tri_c, const float3 p, float &u, float &v)
{
    const float3 v0 = tri_a - tri_c, v1 = tri_b - tri_c, v2 = p - tri_c;
    const float d00 = dot(v0, v0);
    const float d01 = dot(v0, v1);
    const float d11 = dot(v1, v1);
    const float d20 = dot(v2, v0);
    const float d21 = dot(v2, v1);
    float invDenom = 1.0f / (d00 * d11 - d01 * d01);
    u = (d11 * d20 - d01 * d21) * invDenom;
    v = (d00 * d21 - d01 * d20) * invDenom;
}

template<class A, class B>
ccl_device_inline A device_lerp(const A& a, const A& b, const B& t)
{
    return (A)(a * ((B)1 - t) + b * t);
}


ccl_device_inline float3 triangle_normal(const float3 tri_a, const float3 tri_b, const float3 tri_c, int object_flag) {
    if(object_flag & D_SD_OBJECT_NEGATIVE_SCALE_APPLIED) {
        return normalize(cross(tri_c - tri_a, tri_b - tri_a));
    }
    return normalize(cross(tri_b - tri_a, tri_c - tri_a));
}

ccl_device_inline float sdistance_to_plane(const float3 tri_a, const float3 tri_b, const float3 tri_c, int object_flag,  const float3 p) {
    const float3 n = triangle_normal(tri_a, tri_b, tri_c, object_flag);
    return dot (n, p - tri_a);
}

ccl_device_inline void handle_hit_triangle(HitData_2 &min, HitData_2 &max, float3 P, float3 dir, const float3 a, const float3 b, const float3 c, const float t, const float u, const float v, const float w) {
    if (!((t < min.t) || (t > max.t))) {
        return;
    }

    if (!((u + v) <= 1.0f && (u + v) >= 0.0f)) {
        return;
    }

    float3 point = (1.0f - u - v) * c + u * a + v * b;

    if (t < min.t) {
        min.hit = true;
        min.t = t;
        min.u = u;
        min.v = v;
        min.w = w;
        min.p = point;
    }

    if (t > max.t) {
        max.hit = true;
        max.t = t;
        max.u = u;
        max.v = v;
        max.w = w;
        max.p = point;
    }
}

ccl_device_inline bool handle_hit_triangle_new(HitData_3 &data, Side side, const float3 a, const float3 b, const float3 c, const float t, const float u, const float v, const float w) {
    if (!((u + v) <= 1.0f && (u + v) >= 0.0f)) {
        return false;
    }

    data.p = (1 - u - v) * c + u * a + v * b;
    data.u = u;
    data.v = v;
    data.w = w;
    data.t = t;
    data.side = side;
    return true;
}

ccl_device_forceinline bool handle_hit_patch_new(HitData_3 &data, Side side,
                                        const float3 q00, const float3 q10, const float3 q11, const float3 q01,
                                        const float3 tri_a, const float3 tri_b, const float3 tri_c,
                                        const float3 tri_n_a, const float3 tri_n_b, const float3 tri_n_c,
                                        float t, float u, float v, Edge e) {
    float3 point = (1.0f - u)*(1.0f - v) * q00 + (1.0f - u)*v * q01 + u * (1.0f - v) * q10 + u*v* q11;
    const float w = v;

    const float3 T_a = device_lerp(tri_a, tri_a + tri_n_a, w);
    const float3 T_b = device_lerp(tri_b, tri_b + tri_n_b, w);
    const float3 T_c = device_lerp(tri_c, tri_c + tri_n_c, w);

    switch (e) {
        case Edge::AB: {
            float len = 0.0f;
            float3 edge = normalize_len(T_b - T_a, &len);
            v = dot(point - T_a, edge) / len;
            u = (1.0f - v);
        }
        break;

        case Edge::BC: {
            float len = 0.0f;
            float3 edge = normalize_len(T_b - T_c, &len);
            v = dot(point - T_c, edge) / len;
            u = 0.0f;
        }
        break;

        case Edge::CA: {
            float len = 0.0f;
            float3 edge = normalize_len(T_a - T_c, &len);
            v = 0.0f;
            u = dot(point - T_c, edge) / len;
        }
        break;
    }

    data.u = u;
    data.v = v;
    data.w = w;
    data.p = point;
    data.t = t;
    data.side = side;

    float d = sdistance_to_plane(T_a, T_b, T_c, 0, point);
    if (std::abs(d) > 1e-4) {
        printf("%f\n", d);
    }
    return true;
}

ccl_device_inline void handle_hit_patch(HitData_2 &min, HitData_2 &max, float3 P, float3 dir,
                                        const float3 q00, const float3 q10, const float3 q11, const float3 q01,
                                        const float3 tri_a, const float3 tri_b, const float3 tri_c,
                                        const float3 tri_n_a, const float3 tri_n_b, const float3 tri_n_c,
                                        float t, float u, float v, Edge e) {
    if (!((t < min.t) || (t > max.t))) {
        return;
    }

    float3 point = (1.0f - u)*(1.0f - v) * q00 + (1.0f - u)*v * q01 + u * (1.0f - v) * q10 + u*v* q11;
    const float w = v;

    const float3 T_a = device_lerp(tri_a, tri_a + tri_n_a, w);
    const float3 T_b = device_lerp(tri_b, tri_b + tri_n_b, w);
    const float3 T_c = device_lerp(tri_c, tri_c + tri_n_c, w);

    switch (e) {
        case Edge::AB: {
            float len = 0.0f;
            float3 edge = normalize_len(T_b - T_a, &len);
            v = dot(point - T_a, edge) / len;
            u = (1.0f - v);
        }
        break;

        case Edge::BC: {
            float len = 0.0f;
            float3 edge = normalize_len(T_b - T_c, &len);
            v = dot(point - T_c, edge) / len;
            u = 0.0f;
        }
        break;

        case Edge::CA: {
            float len = 0.0f;
            float3 edge = normalize_len(T_a - T_c, &len);
            v = 0.0f;
            u = dot(point - T_c, edge) / len;
        }
        break;
    }

    if (!((u + v) <= 1.0f && (u + v) >= 0.0f)) {
        return;
    }

    if (t < min.t) {
        min.hit = true;
        min.t = t;
        min.u = u;
        min.v = v;
        min.w = w;
        min.p = point;
    }

    if (t > max.t) {
        max.hit = true;
        max.t = t;
        max.u = u;
        max.v = v;
        max.w = w;
        max.p = point;
    }

    float d = sdistance_to_plane(T_a, T_b, T_c, 0, point);
    if (std::abs(d) > 1e-5f) {
        printf("%f\n", d);
    }
}

ccl_device_inline void handle_hit(HitData &min, HitData &max, float3 P, float3 dir, float t, float u, float v, float3 a, float3 b, float3 c) {
    if (!((t < min.t) || (t > max.t))) {
        return;
    }

    float3 point = (1 - u - v) * c + u * a + v * b;
    if (t < min.t) {
        min.hit = true;
        min.t = t;
        min.isUVWSet = false;
        min.p = point;
    }

    if (t > max.t) {
        max.hit = true;
        max.t = t;
        max.isUVWSet = false;
        max.p = point;
    }
}

#if defined(__KERNEL_CUDA__) && __CUDA_ARCH__ < 300
ccl_device_inline
#else
ccl_device_forceinline
#endif
bool ray_triangle_intersect_allow_backtracing(
        float3 ray_P, float3 ray_dir,
    const float3 tri_a, const float3 tri_b, const float3 tri_c,
    float *isect_u, float *isect_v, float *isect_t)
{
    const float3 P = ray_P;
    const float3 dir = ray_dir;

    /* Calculate vertices relative to ray origin. */
    const float3 v0 = tri_c - P;
    const float3 v1 = tri_a - P;
    const float3 v2 = tri_b - P;

    /* Calculate triangle edges. */
    const float3 e0 = v2 - v0;
    const float3 e1 = v0 - v1;
    const float3 e2 = v1 - v2;


    const float U = dot(cross(v2 + v0, e0), ray_dir);
    const float V = dot(cross(v0 + v1, e1), ray_dir);
    const float W = dot(cross(v1 + v2, e2), ray_dir);
    const float minUVW = min(U, min(V, W));
    const float maxUVW = max(U, max(V, W));

    if(minUVW < 0.0f && maxUVW > 0.0f) {
        return false;
    }

    /* Calculate geometry normal and denominator. */
    const float3 Ng1 = cross(e1, e0);
    //const Vec3vfM Ng1 = stable_triangle_normal(e2,e1,e0);
    const float3 Ng = Ng1 + Ng1;
    const float den = dot(Ng, dir);
    /* Avoid division by 0. */
    if(UNLIKELY(den == 0.0f)) {
        return false;
    }

    /* Perform depth test. */
    const float T = dot(v0, Ng);
    const float inv_den = 1.0f / den;
    const float u = U * inv_den;
    const float v = V * inv_den;
    if ((u + v) >= 0.0f && (u + v) <= 1.0f) {
        *isect_u = U * inv_den;
        *isect_v = V * inv_den;
        *isect_t = T * inv_den;
        return true;
    }
    return false;
}

//
//ccl_device_inline void barycentric(const float3 tri_a, const float3 tri_b, const float3 tri_c, const float3 p, float &u, float &v)
//{
//    float3 N = cross(tri_c - tri_b, tri_b - tri_a);
//    float A = len(N);
//    const float3 n = normalize(N);
//    u = len(cross(tri_c - tri_b, p - tri_b)) / A;
//    v = len(cross(tri_a - tri_c, p - tri_c)) / A;
//}


ccl_device_inline float calculate_dot(const float3 tri_a, const float3 tri_b, const float3 tri_c,
                                      const float3 tri_n_a, const float3 tri_n_b, const float3 tri_n_c,
                                      const float3 s_normal,
                                      int object_flag,
                                      const float3 point, const float w) {
    const float3 T_a = device_lerp(tri_a, tri_a + tri_n_a, w);
    const float3 T_b = device_lerp(tri_b, tri_b + tri_n_b, w);
    const float3 T_c = device_lerp(tri_c, tri_c + tri_n_c, w);
    const float3 n = triangle_normal(T_a, T_b, T_c, object_flag);
    float result = dot(n, point - T_a);
    return result;
}

ccl_device_inline float3 calculate_uvw(const float3 tri_a, const float3 tri_b, const float3 tri_c,
                                       const float3 tri_n_a, const float3 tri_n_b, const float3 tri_n_c,
                                       int object_flag,
                                       const float3 point, const float w) {
    const float3 T_a = device_lerp(tri_a, tri_a + tri_n_a, w);
    const float3 T_b = device_lerp(tri_b, tri_b + tri_n_b, w);
    const float3 T_c = device_lerp(tri_c, tri_c + tri_n_c, w);
    const float3 n = triangle_normal(T_a, T_b, T_c, object_flag);
    float3 v0 = point - (T_a + T_b + T_c) / 3.0f;
    float3 proj = point - dot(n, v0) * n;
    float u, v;
    barycentric(T_a, T_b, T_c, proj, u, v);
    return make_float3(u, v , w);
}

// https://link.springer.com/content/pdf/10.1007%2F978-1-4842-4427-2_8.pdf
// q01 ----------- q11
// |                 |
// | e00         e11 |
// |       e10       |
// q00 ----------- q10
ccl_device_inline bool ray_patch_intersection(const float3 origin, const float3 dir, const float t,
                                              float3 q00, float3 q10, float3 q11, float3 q01,
                                              float *isect_u, float *isect_v, float *isect_t) {
    // 4 corners + "normal" qn
    float3 qn = cross(q10-q00, q01-q11);
    float3 e10 = q10 - q00;
    float3 e11 = q11 - q10;
    float3 e00 = q01 - q00;
    q00 -= origin;
    q10 -= origin;
    float a = dot(cross(q00, dir), e00);
    float c = dot(qn, dir);                 // a + b u + c u^2
    float b = dot(cross(q10, dir), e11);    // first compute
    b -= a + c;                             // a+b+c and then b
    float det = b*b - 4.0f*a*c;
    if (det < 0.0f) return false;           // see the right part of Figure 5
    det = sqrt(det);                        // we -use_fast_math in CUDA_NVRTC_OPTIONS
    float u1, u2;                           // two roots(u parameter)
    float min_t = FLT_MAX, u, v;            // need solution for the smallest t > 0
    if (c == 0.0f) {                        // if c == 0, it is a trapezoid
        u1 = -a/b; u2 = -1;                 // and there is only one root
    } else {                                // (c != 0 in Stanford models)
        u1 = (-b - copysignf(det, b))/2.0f;    // numerically "stable" root
        u2 = a/u1;                          // Viete's formula for u1*u2
        u1 /= c;
    }
    if (0.0f <= u1 && u1 <= 1.0f) {                  // is it inside the patch?
        float3 pa = device_lerp(q00, q10, u1);        // point on edge e10 (Fig. 4)
        float3 pb = device_lerp(e00, e11, u1);        // it is, actually, pb - pa
        float3 n = cross(dir, pb);
        det = dot(n, n);
        n = cross(n, pa);
        float t1 = dot(n, pb);
        float v1 = dot(n, dir);
        if (t1 > 0.0f && 0.0f <= v1 && v1 <= det) {  // if t1 > t,
            min_t = t1/det; u = u1; v = v1/det;      // it will be rejected
        }                                            // in the end
    }

    if (0.0f <= u2 && u2 <= 1.0f) {                   // it is slightly different,
        float3 pa = device_lerp(q00, q10, u2);        // since u1 might be good
        float3 pb = device_lerp(e00, e11, u2);        // and we need 0 < t2 < t1
        float3 n = cross(dir, pb);
        det = dot(n, n);
        n = cross(n, pa);
        float t2 = dot(n, pb)/det;
        float v2 = dot(n, dir);
        if (0.0f <= v2 && v2 <= det && min_t > t2 && t2 > 0.0f) {
            min_t = t2; u = u2; v = v2/det;
        }
    }

    if (min_t < t) {
        *isect_t = min_t;
        *isect_u = u;
        *isect_v = v;
        return true;
    }
    return false;
}

ccl_device_inline bool ray_patch_intersection_allow_backtracing(const float3 origin, const float3 dir,
                                                                float3 q00, float3 q10, float3 q11, float3 q01,
                                                                float *isect1_u, float *isect1_v, float *isect1_t,
                                                                float *isect2_u, float *isect2_v, float *isect2_t,
                                                                int *num_intersections) {
    // 4 corners + "normal" qn
    float3 qn = cross(q10-q00, q01-q11);
    float3 e10 = q10 - q00;
    float3 e11 = q11 - q10;
    float3 e00 = q01 - q00;
    q00 -= origin;
    q10 -= origin;
    float a = dot(cross(q00, dir), e00);
    float c = dot(qn, dir);                 // a + b u + c u^2
    float b = dot(cross(q10, dir), e11);    // first compute
    b -= a + c;                             // a+b+c and then b
    float det = b*b - 4.0f*a*c;
    if (det < 0.0f) return false;           // see the right part of Figure 5
    det = sqrt(det);                        // we -use_fast_math in CUDA_NVRTC_OPTIONS
    float u1, u2, t = FLT_MAX;                           // two roots(u parameter)
    *num_intersections = 0;
    if (c == 0.0f) {                        // if c == 0, it is a trapezoid
        u1 = -a/b; u2 = -1;                 // and there is only one root
    } else {                                // (c != 0 in Stanford models)
        u1 = (-b - copysignf(det, b))/2.0f;    // numerically "stable" root
        u2 = a/u1;                          // Viete's formula for u1*u2
        u1 /= c;
    }
    if (0.0f <= u1 && u1 <= 1.0f) {                  // is it inside the patch?
        float3 pa = device_lerp(q00, q10, u1);        // point on edge e10 (Fig. 4)
        float3 pb = device_lerp(e00, e11, u1);        // it is, actually, pb - pa
        float3 n = cross(dir, pb);
        det = dot(n, n);
        n = cross(n, pa);
        float t1 = dot(n, pb);
        float v1 = dot(n, dir);
        if (0.0f <= v1 && v1 <= det) {  // if t1 > t,
            t = t1;
            *num_intersections = 1;
            *isect1_t = t1/det;
            *isect1_u = u1;
            *isect1_v = v1/det;
        }
    }

    if (0.0f <= u2 && u2 <= 1.0f) {                   // it is slightly different,
        float3 pa = device_lerp(q00, q10, u2);        // since u1 might be good
        float3 pb = device_lerp(e00, e11, u2);        // and we need 0 < t2 < t1
        float3 n = cross(dir, pb);
        det = dot(n, n);
        n = cross(n, pa);
        float t2 = dot(n, pb)/det;
        float v2 = dot(n, dir);
        if (0.0f <= v2 && v2 <= det && t2 != t) {
            if (*num_intersections == 1) {
                if (t2 > *isect1_t) {
                    *isect2_t = t2;
                    *isect2_u = u2;
                    *isect2_v = v2/det;
                } else {
                    *isect2_t = *isect1_t;
                    *isect2_u = *isect1_u;
                    *isect2_v = *isect1_v;

                    *isect1_t = t2;
                    *isect1_u = u2;
                    *isect1_v = v2/det;
                }
            } else {
                *isect1_t = t2;
                *isect1_u = u2;
                *isect1_v = v2/det;
            }
            ++(*num_intersections);
        }
    }

    return (*num_intersections > 0);
}

ccl_device_inline float3 prism_point_uvw_coords(const float3 tri_a, const float3 tri_b, const float3 tri_c,
                                                const float3 tri_n_a, const float3 tri_n_b, const float3 tri_n_c,
                                                int object_flag,
                                                const float3 point, const float EPS) {
    float3 s_normal = triangle_normal(tri_a, tri_b, tri_c, object_flag);
    float w = 0.0f;
    float min = 0.0f;
    float max = 1.0f;

    float f_min_dot = calculate_dot(tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c, s_normal, object_flag, point, min);

    if ((std::abs(f_min_dot) <  EPS)) {
        return calculate_uvw(tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c, object_flag, point, min);
    }

    float f_max_dot = calculate_dot(tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c, s_normal, object_flag, point, max);
    if ((std::abs(f_max_dot) <  EPS)) {
        return calculate_uvw(tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c, object_flag, point, max);
    }

    // usually 12 iterations are enough
    float f_mid_dot = 0.0f;
    for (int i = 0; i < 40; ++i) {
        w = 0.5f * min + 0.5f * max;

        f_mid_dot = calculate_dot(tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c, s_normal, object_flag, point, w);

        if (std::abs(f_mid_dot) < EPS) {
            break;
        }

        if ((f_min_dot < 0.0f) == (f_mid_dot < 0.0f)) {
            f_min_dot = f_mid_dot;
            min = w;
        } else {
            max = w;
        }
    }

//    if (f_mid_dot >= 0.01f) {
//        printf("triangle __int_as_float(%d), __int_as_float(%d), __int_as_float(%d) | __int_as_float(%d), __int_as_float(%d), __int_as_float(%d) | __int_as_float(%d), __int_as_float(%d), __int_as_float(%d)\nnormals __int_as_float(%d), __int_as_float(%d), __int_as_float(%d) | __int_as_float(%d), __int_as_float(%d), __int_as_float(%d) | __int_as_float(%d), __int_as_float(%d), __int_as_float(%d)\nprism_point_uvw_coords: __int_as_float(%d), __int_as_float(%d), __int_as_float(%d) dot %f\n",
//                __float_as_int(tri_a.x), __float_as_int(tri_a.y), __float_as_int(tri_a.z),
//               __float_as_int(tri_b.x), __float_as_int(tri_b.y), __float_as_int(tri_b.z),
//                              __float_as_int(tri_c.x), __float_as_int(tri_c.y), __float_as_int(tri_c.z),
//                                             __float_as_int(tri_n_a.x), __float_as_int(tri_n_a.y), __float_as_int(tri_n_a.z),
//                                                            __float_as_int(tri_n_b.x), __float_as_int(tri_n_b.y), __float_as_int(tri_n_b.z),
//                                                                           __float_as_int(tri_n_c.x), __float_as_int(tri_n_c.y), __float_as_int(tri_n_c.z),
//                                                                           __float_as_int(point.x), __float_as_int(point.y), __float_as_int(point.z), (double)f_mid_dot);
//    }

    if (std::abs(f_mid_dot) >= 10 * EPS) {
        printf("triangle %f %f %f | %f %f %f | %f %f %f\nnormals %f %f %f | %f %f %f | %f %f %f\nprism_point_uvw_coords: %f %f %f dot %f\n",
               tri_a.x, tri_a.y, tri_a.z,
               tri_b.x, tri_b.y, tri_b.z,
               tri_c.x, tri_c.y, tri_c.z,
               tri_n_a.x, tri_n_a.y, tri_n_a.z,
               tri_n_b.x, tri_n_b.y, tri_n_b.z,
               tri_n_c.x, tri_n_c.y, tri_n_c.z,
               (double)point.x, (double)point.y, (double)point.z, (double)f_mid_dot);
    }

    return calculate_uvw(tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c, object_flag, point, w);
}
//
//ccl_device_forceinline bool common_part_geopattern_find_intersections(const float3 P,
//                                                                      const float3 dir,
//                                                                      const float ray_t,
//                                                                      const float t_near,
//                                                                      const float t_far,
//                                                                      int prim_addr) {
//    const int prim = kernel_tex_fetch(__prim_index, prim_addr);
//    const uint4 tri_vindex = kernel_tex_fetch(__tri_vindex, prim);
//    int object_flag = kernel_tex_fetch(__object_flag, object_idx);
//
//    float3 tri_a = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 0));
//    float3 tri_b = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 1));
//    float3 tri_c = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 2));
//    float3 tri_n_a = normalize(float4_to_float3(kernel_tex_fetch(__tri_vnormal, tri_vindex.x)));
//    float3 tri_n_b = normalize(float4_to_float3(kernel_tex_fetch(__tri_vnormal, tri_vindex.y)));
//    float3 tri_n_c = normalize(float4_to_float3(kernel_tex_fetch(__tri_vnormal, tri_vindex.z)));
//
//    tri_n_a *= geopatten_height;
//    tri_n_b *= geopatten_height;
//    tri_n_c *= geopatten_height;
//
//    float3 &prism_tri_n_a = tri_n_a;
//    float3 &prism_tri_n_b = tri_n_b;
//    float3 &prism_tri_n_c = tri_n_c;
//
//    float3 &prism_tri_1_a = tri_a;
//    float3 &prism_tri_1_b = tri_b;
//    float3 &prism_tri_1_c = tri_c;
//
//    float3 prism_tri_2_a = prism_tri_1_a + prism_tri_n_a;
//    float3 prism_tri_2_b = prism_tri_1_b + prism_tri_n_b;
//    float3 prism_tri_2_c = prism_tri_1_c + prism_tri_n_c;
//
//    constexpr size_t hit_size = 8;
//    HitData_3 hit[hit_size];
//    float u, v, t;
//
//    int total_intersection = 0;
//
//    if (ray_triangle_intersect_allow_backtracing(P, dir, prism_tri_1_a, prism_tri_1_b, prism_tri_1_c, &u, &v, &t)) {
//        if (handle_hit_triangle_new(hit[total_intersection], Side::Top, prism_tri_1_a, prism_tri_1_b, prism_tri_1_c, t, u, v, 0.0f)) {
//            total_intersection++;
//        }
//    }
//
//    if (ray_triangle_intersect_allow_backtracing(P, dir, prism_tri_2_a, prism_tri_2_b, prism_tri_2_c, &u, &v, &t)) {
//        if (handle_hit_triangle_new(hit[total_intersection], Side::Down, prism_tri_1_a, prism_tri_1_b, prism_tri_1_c, t, u, v, 1.0f)) {
//            total_intersection++;
//        }
//    }
//
//    // side a, b
//    int num_intersection = 0;
//    float u2 = FLT_MAX, v2 = FLT_MAX, t2 = FLT_MAX;
//    if (ray_patch_intersection_allow_backtracing(P, dir, prism_tri_1_a, prism_tri_1_b, prism_tri_2_b, prism_tri_2_a, &u, &v, &t, &u2, &v2, &t2, &num_intersection)) {
//        if (handle_hit_patch_new(hit[total_intersection], Side::AB,
//                                 prism_tri_1_a, prism_tri_1_b, prism_tri_2_b, prism_tri_2_a,
//                                 tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
//                                 t, u, v, Edge::AB)) {
//            total_intersection++;
//        }
//
//        if (   num_intersection > 1
//               && handle_hit_patch_new(hit[total_intersection], Side::AB,
//                                       prism_tri_1_a, prism_tri_1_b, prism_tri_2_b, prism_tri_2_a,
//                                       tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
//                                       t2, u2, v2, Edge::AB)) {
//            total_intersection++;
//        }
//    }
//
//    // side b, c
//    num_intersection = 0;
//    if (ray_patch_intersection_allow_backtracing(P, dir, prism_tri_1_b, prism_tri_1_c, prism_tri_2_c, prism_tri_2_b, &u, &v, &t, &u2, &v2, &t2, &num_intersection)) {
//        if (handle_hit_patch_new(hit[total_intersection], Side::BC,
//                                 prism_tri_1_b, prism_tri_1_c, prism_tri_2_c, prism_tri_2_b,
//                                 tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
//                                 t, u, v, Edge::BC)) {
//            total_intersection++;
//        }
//
//        if (   num_intersection > 1
//               && handle_hit_patch_new(hit[total_intersection], Side::BC,
//                                       prism_tri_1_b, prism_tri_1_c, prism_tri_2_c, prism_tri_2_b,
//                                       tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
//                                       t2, u2, v2, Edge::BC)) {
//            total_intersection++;
//        }
//    }
//
//    num_intersection = 0;
//    if (ray_patch_intersection_allow_backtracing(P, dir, prism_tri_1_c, prism_tri_1_a, prism_tri_2_a, prism_tri_2_c, &u, &v, &t, &u2, &v2, &t2, &num_intersection)) {
//        if (handle_hit_patch_new(hit[total_intersection], Side::CA,
//                                 prism_tri_1_c, prism_tri_1_a, prism_tri_2_a, prism_tri_2_c,
//                                 tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
//                                 t, u, v, Edge::CA)) {
//            total_intersection++;
//        }
//
//        if (num_intersection > 1
//            && handle_hit_patch_new(hit[total_intersection], Side::CA,
//                                    prism_tri_1_c, prism_tri_1_a, prism_tri_2_a, prism_tri_2_c,
//                                    tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
//                                    t2, u2, v2, Edge::CA)) {
//            total_intersection++;
//        }
//    }
//
//    if (total_intersection > 2) {
//        for (int i = 0; i < total_intersection; i++) {
//            for (int j = i + 1; j < total_intersection; j++) {
//                if (hit[i].t > hit[j].t) {
//                    HitData_3 tmp = hit[i];
//                    hit[i] = hit[j];
//                    hit[j] = tmp;
//                }
//            }
//        }
//
//
//        for (int i = 0; i < total_intersection; i++) {
//
//        }
//
//        if (num_intersection) {
//            ++stack_ptr;
//            assert(stack_ptr < BVH_STACK_SIZE);
//            traversal_stack[stack_ptr] = node_addr;
//
//            ++stack_ptr;
//            assert(stack_ptr < BVH_STACK_SIZE);
//            traversal_stack[stack_ptr] = object;
//
//            ++stack_ptr;
//            assert(stack_ptr < BVH_STACK_SIZE);
//            traversal_stack[stack_ptr] = GEOPATTERN_SENTINEL;
//            node_addr = GEOPATTERN_SENTINEL;
//            object = geopattern_link;
//
//
//        }
//    }
//    return false;
//}

ccl_device_forceinline bool common_part_geopattern_inside(
        const int prim_addr,
        const int object_idx,
        const uint geopattern_link,
        const uint geopattern_clipbox_link,
        const float geopatten_height,
        float3 &P,
        float3 &dir,
        float3 &idir,
        float &ray_t,
        float &t_near,
        float &t_far,

        int &stack_ptr,
        int *traversal_stack,
        int &node_addr,
        int &object,
        Mat3 &T,
        GeopatternFrame &geoframe,
        Intersection *isect)
{
    const int prim = kernel_tex_fetch(__prim_index, prim_addr);
    const uint4 tri_vindex = kernel_tex_fetch(__tri_vindex, prim);
    int object_flag = kernel_tex_fetch(__object_flag, object_idx);


    float3 tri_a = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 0));
    float3 tri_b = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 1));
    float3 tri_c = float4_to_float3(kernel_tex_fetch(__prim_tri_verts, tri_vindex.w + 2));
    float3 tri_n_a = normalize(float4_to_float3(kernel_tex_fetch(__tri_vnormal, tri_vindex.x)));
    float3 tri_n_b = normalize(float4_to_float3(kernel_tex_fetch(__tri_vnormal, tri_vindex.y)));
    float3 tri_n_c = normalize(float4_to_float3(kernel_tex_fetch(__tri_vnormal, tri_vindex.z)));

    //tri_n_a = tri_n_b = tri_n_c = triangle_normal(tri_a, tri_b, tri_c, object_flag);

    tri_n_a *= geopatten_height;
    tri_n_b *= geopatten_height;
    tri_n_c *= geopatten_height;

    float3 &prism_tri_n_a = tri_n_a;
    float3 &prism_tri_n_b = tri_n_b;
    float3 &prism_tri_n_c = tri_n_c;

    float3 &prism_tri_1_a = tri_a;
    float3 &prism_tri_1_b = tri_b;
    float3 &prism_tri_1_c = tri_c;

    float3 prism_tri_2_a = prism_tri_1_a + prism_tri_n_a;
    float3 prism_tri_2_b = prism_tri_1_b + prism_tri_n_b;
    float3 prism_tri_2_c = prism_tri_1_c + prism_tri_n_c;

    constexpr size_t hit_size = 4;
    HitData_3 hit[hit_size * 2];

//    min_hit.hit = false;
//    min_hit.t = FLT_MAX;
//
//    max_hit.hit = false;
//    max_hit.t = -FLT_MAX;
    float u, v, t;

//    int total_intersection = 0;
//
//    if (ray_triangle_intersect_allow_backtracing(P, dir, prism_tri_1_a, prism_tri_1_b, prism_tri_1_c, &u, &v, &t)) {
//        if (isect->t > t && t > 0) {
//            isect->t = t;
//            isect->u = u;
//            isect->v = v;
//            isect->type = PRIMITIVE_TRIANGLE;
//            isect->prim = prim_addr;
//            isect->object = object;
//        }
//
//        if (handle_hit_triangle_new(hit[total_intersection], Side::Top, prism_tri_1_a, prism_tri_1_b, prism_tri_1_c, t, u, v, 0.0f)) {
//            total_intersection++;
//        }
//    }
//
//    if (ray_triangle_intersect_allow_backtracing(P, dir, prism_tri_2_a, prism_tri_2_b, prism_tri_2_c, &u, &v, &t)) {
//        if (isect->t > t && t > 0) {
//            isect->t = t;
//            isect->u = u;
//            isect->v = v;
//            isect->type = PRIMITIVE_TRIANGLE;
//            isect->prim = prim_addr;
//            isect->object = object;
//        }
//
//        if (handle_hit_triangle_new(hit[total_intersection], Side::Down, prism_tri_1_a, prism_tri_1_b, prism_tri_1_c, t, u, v, 1.0f)) {
//            total_intersection++;
//        }
//    }
//
//    // side a, b
//    int num_intersection = 0;
//    float u2 = FLT_MAX, v2 = FLT_MAX, t2 = FLT_MAX;
//    if (ray_patch_intersection_allow_backtracing(P, dir, prism_tri_1_a, prism_tri_1_b, prism_tri_2_b, prism_tri_2_a, &u, &v, &t, &u2, &v2, &t2, &num_intersection)) {
//        if (isect->t > t && t > 0) {
//            isect->t = t;
//            isect->u = u;
//            isect->v = v;
//            isect->type = PRIMITIVE_TRIANGLE;
//            isect->prim = prim_addr;
//            isect->object = object;
//        }
//
//        if (isect->t > t2 && t2 > 0) {
//            isect->t = t2;
//            isect->u = u2;
//            isect->v = v2;
//            isect->type = PRIMITIVE_TRIANGLE;
//            isect->prim = prim_addr;
//            isect->object = object;
//        }
//
//        if (handle_hit_patch_new(hit[total_intersection], Side::AB,
//                         prism_tri_1_a, prism_tri_1_b, prism_tri_2_b, prism_tri_2_a,
//                         tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
//                         t, u, v, Edge::AB)) {
//            total_intersection++;
//        }
//
//        if (   num_intersection > 1
//            && handle_hit_patch_new(hit[total_intersection], Side::AB,
//                             prism_tri_1_a, prism_tri_1_b, prism_tri_2_b, prism_tri_2_a,
//                             tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
//                             t2, u2, v2, Edge::AB)) {
//            total_intersection++;
//        }
//    }
//
//    // side b, c
//    num_intersection = 0;
//    if (ray_patch_intersection_allow_backtracing(P, dir, prism_tri_1_b, prism_tri_1_c, prism_tri_2_c, prism_tri_2_b, &u, &v, &t, &u2, &v2, &t2, &num_intersection)) {
//        if (isect->t > t2 && t2 > 0) {
//            isect->t = t2;
//            isect->u = u2;
//            isect->v = v2;
//            isect->type = PRIMITIVE_TRIANGLE;
//            isect->prim = prim_addr;
//            isect->object = object;
//        }
//
//        if (isect->t > t && t > 0) {
//            isect->t = t;
//            isect->u = u;
//            isect->v = v;
//            isect->type = PRIMITIVE_TRIANGLE;
//            isect->prim = prim_addr;
//            isect->object = object;
//        }
//
//        if (handle_hit_patch_new(hit[total_intersection], Side::BC,
//                         prism_tri_1_b, prism_tri_1_c, prism_tri_2_c, prism_tri_2_b,
//                         tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
//                         t, u, v, Edge::BC)) {
//            total_intersection++;
//        }
//
//        if (   num_intersection > 1
//            && handle_hit_patch_new(hit[total_intersection], Side::BC,
//                             prism_tri_1_b, prism_tri_1_c, prism_tri_2_c, prism_tri_2_b,
//                             tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
//                             t2, u2, v2, Edge::BC)) {
//            total_intersection++;
//        }
//    }
//
//    num_intersection = 0;
//    if (ray_patch_intersection_allow_backtracing(P, dir, prism_tri_1_c, prism_tri_1_a, prism_tri_2_a, prism_tri_2_c, &u, &v, &t, &u2, &v2, &t2, &num_intersection)) {
//        if (isect->t > t && t > 0) {
//            isect->t = t;
//            isect->u = u;
//            isect->v = v;
//            isect->type = PRIMITIVE_TRIANGLE;
//            isect->prim = prim_addr;
//            isect->object = object;
//        }
//
//        if (isect->t > t2 && t2 > 0) {
//            isect->t = t2;
//            isect->u = u2;
//            isect->v = v2;
//            isect->type = PRIMITIVE_TRIANGLE;
//            isect->prim = prim_addr;
//            isect->object = object;
//        }
//
//        if (handle_hit_patch_new(hit[total_intersection], Side::CA,
//                         prism_tri_1_c, prism_tri_1_a, prism_tri_2_a, prism_tri_2_c,
//                         tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
//                         t, u, v, Edge::CA)) {
//            total_intersection++;
//        }
//
//        if (num_intersection > 1
//            && handle_hit_patch_new(hit[total_intersection], Side::CA,
//                             prism_tri_1_c, prism_tri_1_a, prism_tri_2_a, prism_tri_2_c,
//                             tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
//                             t2, u2, v2, Edge::CA)) {
//            total_intersection++;
//        }
//    }
//
//    for (int i = 0; i < total_intersection; i++) {
//        for (int j = i + 1; j < total_intersection; j++) {
//            if (hit[i].t > hit[j].t) {
//                HitData_3 tmp = hit[i];
//                hit[i] = hit[j];
//                hit[j] = tmp;
//            }
//        }
//    }

    HitData_2 min_hit;
    min_hit.hit = false;
    min_hit.t = FLT_MAX;

    HitData_2 max_hit;
    max_hit.hit = false;
    max_hit.t = -FLT_MAX;


    int total_intersection = 0;

    if (ray_triangle_intersect_allow_backtracing(P, dir, prism_tri_1_a, prism_tri_1_b, prism_tri_1_c, &u, &v, &t)) {
        total_intersection++;
        handle_hit_triangle(min_hit, max_hit, P, dir, prism_tri_1_a, prism_tri_1_b, prism_tri_1_c, t, u, v, 0.0f);
    }

    if (ray_triangle_intersect_allow_backtracing(P, dir, prism_tri_2_a, prism_tri_2_b, prism_tri_2_c, &u, &v, &t)) {
        total_intersection++;
        handle_hit_triangle(min_hit, max_hit, P, dir, prism_tri_2_a, prism_tri_2_b, prism_tri_2_c, t, u, v, 1.0f);
    }

    // side a, b
    int num_intersection = 0;
    float u2 = FLT_MAX, v2 = FLT_MAX, t2 = FLT_MAX;
    if (ray_patch_intersection_allow_backtracing(P, dir, prism_tri_1_a, prism_tri_1_b, prism_tri_2_b, prism_tri_2_a, &u, &v, &t, &u2, &v2, &t2, &num_intersection)) {
        total_intersection++;
        handle_hit_patch(min_hit, max_hit, P, dir,
                             prism_tri_1_a, prism_tri_1_b, prism_tri_2_b, prism_tri_2_a,
                         tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
                         t, u, v, Edge::AB);

        if (num_intersection > 1) {
            total_intersection++;
            handle_hit_patch(min_hit, max_hit, P, dir,
                                 prism_tri_1_a, prism_tri_1_b, prism_tri_2_b, prism_tri_2_a,
                                 tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
                                 t2, u2, v2, Edge::AB);
        }
    }

    // side b, c
    num_intersection = 0;
    if (ray_patch_intersection_allow_backtracing(P, dir, prism_tri_1_b, prism_tri_1_c, prism_tri_2_c, prism_tri_2_b, &u, &v, &t, &u2, &v2, &t2, &num_intersection)) {
        total_intersection++;
        handle_hit_patch(min_hit, max_hit, P, dir,
                         prism_tri_1_b, prism_tri_1_c, prism_tri_2_c, prism_tri_2_b,
                         tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
                         t, u, v, Edge::BC);

        if (num_intersection > 1) {
            total_intersection++;
            handle_hit_patch(min_hit, max_hit, P, dir,
                             prism_tri_1_b, prism_tri_1_c, prism_tri_2_c, prism_tri_2_b,
                             tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
                             t2, u2, v2, Edge::BC);
        }
    }

    num_intersection = 0;
    if (ray_patch_intersection_allow_backtracing(P, dir, prism_tri_1_c, prism_tri_1_a, prism_tri_2_a, prism_tri_2_c, &u, &v, &t, &u2, &v2, &t2, &num_intersection)) {
        total_intersection++;
        handle_hit_patch(min_hit, max_hit, P, dir,
                         prism_tri_1_c, prism_tri_1_a, prism_tri_2_a, prism_tri_2_c,
                         tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
                         t, u, v, Edge::CA);

        if (num_intersection > 1) {
            total_intersection++;
            handle_hit_patch(min_hit, max_hit, P, dir,
                             prism_tri_1_c, prism_tri_1_a, prism_tri_2_a, prism_tri_2_c,
                             tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
                             t2, u2, v2, Edge::CA);
        }
    }

//    if (ray_triangle_intersect(P, dir, FLT_MAX, prism_tri_1_a, prism_tri_1_b, prism_tri_1_c, &u, &v, &t)) {
//        total_intersection++;
//        handle_hit_triangle(min_hit, max_hit,  dir, P, prism_tri_1_a, prism_tri_1_b, prism_tri_1_c, t, u, v, 0.0f);
//    }
//
//    if (ray_triangle_intersect(P, dir, FLT_MAX, prism_tri_2_a, prism_tri_2_b, prism_tri_2_c, &u, &v, &t)) {
//        total_intersection++;
//        handle_hit_triangle(min_hit, max_hit,  dir, P, prism_tri_2_a, prism_tri_2_b, prism_tri_2_c, t, u, v, 1.0f);
//    }
//
//    // side a, b
//    if (ray_patch_intersection(P, dir,  FLT_MAX, prism_tri_1_a, prism_tri_1_b, prism_tri_2_b, prism_tri_2_a, &u, &v, &t)) {
//        total_intersection++;
//        handle_hit_patch(min_hit, max_hit, dir, P,
//                         prism_tri_1_a, prism_tri_1_b, prism_tri_2_b, prism_tri_2_a,
//                         tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
//                         t, u, v, Edge::AB);
//    }
//
//    // side b, c
//    if (ray_patch_intersection(P, dir,  FLT_MAX,  prism_tri_1_b, prism_tri_1_c, prism_tri_2_c, prism_tri_2_b, &u, &v, &t)) {
//        total_intersection++;
//        handle_hit_patch(min_hit, max_hit, dir, P,
//                         prism_tri_1_b, prism_tri_1_c, prism_tri_2_c, prism_tri_2_b,
//                         tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
//                         t, u, v, Edge::BC);
//    }
//
//    if (ray_patch_intersection(P, dir,  FLT_MAX,  prism_tri_1_c, prism_tri_1_a, prism_tri_2_a, prism_tri_2_c, &u, &v, &t)) {
//        total_intersection++;
//        handle_hit_patch(min_hit, max_hit, dir, P,
//                         prism_tri_1_c, prism_tri_1_a, prism_tri_2_a, prism_tri_2_c,
//                         tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
//                         t, u, v, Edge::CA);
//    }

//    for (int i = 0; i < total_intersection; i++) {
//        for (int j = i + 1; j < total_intersection; j++) {
//            if (hit[i].t > hit[j].t) {
//                HitData_3 tmp = hit[i];
//                hit[i] = hit[j];
//                hit[j] = tmp;
//            }
//        }
//    }

//    for (int i = 0; i < total_intersection - 1; i++) {
//        if (hit[i].t < ray_t) {
//            min_hit = hit[i];
//            max_hit = hit[i + 1];
//            break;
//        }
//    }

    if (total_intersection > 2) {
//        min_hit = hit[0];
//        max_hit = hit[total_intersection - 1];

        printf("%d %d %f %d %f %d %f %d %f\n", total_intersection,
                hit[0].side, hit[0].t,
                hit[1].side, hit[1].t,
                hit[2].side, hit[2].t,
                hit[3].side, hit[3].t);
    }
//
    if (min_hit.hit && max_hit.hit
        && ((min_hit.t > 0 && ray_t > min_hit.t && min_hit.t < max_hit.t)
            || (min_hit.t < 0.0f && max_hit.t > 0.0f))) {

        bool go_inside = true;

        if (min_hit.t < 0.0f) {
////            float t = -min_hit.t / (max_hit.t - min_hit.t);
////            float w = device_lerp(min_hit.w, max_hit.w, t);
////
////            const float3 T_a = device_lerp(tri_a, tri_a + tri_n_a, w);
////            const float3 T_b = device_lerp(tri_b, tri_b + tri_n_b, w);
////            const float3 T_c = device_lerp(tri_c, tri_c + tri_n_c, w);
////
////            barycentric(T_a, T_b, T_c, P, u, v);
////
////            float d = sdistance_to_plane(T_a, T_b, T_c, 0, P);
////            if (std::abs(d) > 1e-4) {
////                printf("%f\n", d);
////            }
////
////            if (!((u + v) <= 1.0f && (u + v) >= 0.0f)) {
////                go_inside = false;
////            }
////
////            min_hit.p = P;
////            min_hit.t = 0.0f;
////            min_hit.u = u;
////            min_hit.v = v;
////            min_hit.w = w;
////
////
//            float3 uvw = prism_point_uvw_coords(tri_a, tri_b, tri_c,
//                    tri_n_a, tri_n_b, tri_n_c,
//                    object_flag, P, EPS);
//            if (!((uvw.x + uvw.y) <= 1.0f && (uvw.x + uvw.y) >= 0.0f)) {
//                go_inside = false;
//            }
//
//            min_hit.p = P;
//            min_hit.t = 0.0f;
//            min_hit.u = uvw.x;
//            min_hit.v = uvw.y;
//            min_hit.w = uvw.z;
//
//            const float3 T_a = device_lerp(tri_a, tri_a + tri_n_a, min_hit.w);
//            const float3 T_b = device_lerp(tri_b, tri_b + tri_n_b, min_hit.w);
//            const float3 T_c = device_lerp(tri_c, tri_c + tri_n_c, min_hit.w);
//            float d = sdistance_to_plane(T_a, T_b, T_c, 0, P);
//            if (std::abs(d) > 1e-4f) {
//                printf("%f\n", d);
//            }
        }

        if (go_inside) {
           // printf("%f %f \n", min_hit.t, max_hit.t);
            ++stack_ptr;
            assert(stack_ptr < BVH_STACK_SIZE);
            traversal_stack[stack_ptr] = node_addr;

            ++stack_ptr;
            assert(stack_ptr < BVH_STACK_SIZE);
            traversal_stack[stack_ptr] = object;

            ++stack_ptr;
            assert(stack_ptr < BVH_STACK_SIZE);
            traversal_stack[stack_ptr] = GEOPATTERN_SENTINEL;

            object = geopattern_link;

            node_addr = kernel_tex_fetch(__object_node, object);
            assert(node_addr >= 0);
            float4 xx = kernel_tex_fetch(__bvh_nodes, node_addr + 1);
            float4 yy = kernel_tex_fetch(__bvh_nodes, node_addr + 2);
            float4 zz = kernel_tex_fetch(__bvh_nodes, node_addr + 3);

            // Отступы нужны
            float3 bbox_min = {
                    min4(xx.x, xx.y, xx.z, xx.w),// + shift_eps,
                    min4(yy.x, yy.y, yy.z, yy.w),// + shift_eps,
                    min4(zz.x, zz.y, zz.z, zz.w)// - shift_eps
            };

            float3 bbox_max = {
                    max4(xx.x, xx.y, xx.z, xx.w),// - shift_eps,
                    max4(yy.x, yy.y, yy.z, yy.w),// - shift_eps,
                    max4(zz.x, zz.y, zz.z, zz.w)
            };

            if (bbox_max.z == bbox_min.z) {
                bbox_max.z += geopatten_height;
            } else {
                /*bbox_min.x -= shift_eps;
                bbox_max.x += shift_eps;
                bbox_min.y -= shift_eps;
                bbox_max.y += shift_eps;*/
                bbox_min.z -= shift_eps;
                bbox_max.z += shift_eps;
            }

            const float2 tri_a_uv = kernel_tex_fetch(__prim_tri_uv_geopattern, tri_vindex.w + 0);
            const float2 tri_b_uv = kernel_tex_fetch(__prim_tri_uv_geopattern, tri_vindex.w + 1);
            const float2 tri_c_uv = kernel_tex_fetch(__prim_tri_uv_geopattern, tri_vindex.w + 2);

            float height1 = device_lerp(bbox_min.z, bbox_max.z, min_hit.w);
            float height2 = device_lerp(bbox_min.z, bbox_max.z, max_hit.w);
            float3 A_uv = make_float3(device_lerp(bbox_min.x, bbox_max.x, tri_a_uv.x),
                                      device_lerp(bbox_min.y, bbox_max.y, tri_a_uv.y),
                                      height1);

            float3 B_uv = make_float3(device_lerp(bbox_min.x, bbox_max.x, tri_b_uv.x),
                                      device_lerp(bbox_min.y, bbox_max.y, tri_b_uv.y),
                                      height1);

            float3 C_uv = make_float3(device_lerp(bbox_min.x, bbox_max.x, tri_c_uv.x),
                                      device_lerp(bbox_min.y, bbox_max.y, tri_c_uv.y),
                                      height1);

            float3 A2_uv = make_float3(device_lerp(bbox_min.x, bbox_max.x, tri_a_uv.x),
                                      device_lerp(bbox_min.y, bbox_max.y, tri_a_uv.y),
                                      height2);

            float3 B2_uv = make_float3(device_lerp(bbox_min.x, bbox_max.x, tri_b_uv.x),
                                       device_lerp(bbox_min.y, bbox_max.y, tri_b_uv.y),
                                       height2);

            float3 C2_uv = make_float3(device_lerp(bbox_min.x, bbox_max.x, tri_c_uv.x),
                                       device_lerp(bbox_min.y, bbox_max.y, tri_c_uv.y),
                                       height2);

            float2 uv_s = (1.0f - min_hit.u - min_hit.v) * tri_c_uv + min_hit.u * tri_a_uv + min_hit.v * tri_b_uv;
            float2 uv_e = (1.0f - max_hit.u - max_hit.v) * tri_c_uv + max_hit.u * tri_a_uv + max_hit.v * tri_b_uv;
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


            //fabs(min_hit.p.z - max_hit.p.z)
            float3 Ng = geopatten_height * normalize((tri_n_a + tri_n_b + tri_n_c) / 3.0f);
            Q[0][2] = Ng.x;
            Q[1][2] = Ng.y;
            Q[2][2] = Ng.z;

            float3 e1 = (B_uv - A_uv);
            float3 e2 = (C_uv - A_uv);

            QQ[0][0] = e1.x;
            QQ[1][0] = e1.y;
            QQ[2][0] = 0;

            QQ[0][1] = e2.x;
            QQ[1][1] = e2.y;
            QQ[2][1] = 0;

            //float3 N = triangle_normal(A_uv, B_uv, C_uv, 0);
            /*if ((bbox_max.z - bbox_min.z) > 1.0) {
                N *= fabs(height2 - height1);
            }*/

            QQ[0][2] = 0;
            QQ[1][2] = 0;
            // fabs(height2 - height1)
            QQ[2][2] = ((bbox_max.z - bbox_min.z) > 0.0)? (bbox_max.z - bbox_min.z) : 1.0f;

            T = Q * inverse(QQ);

            float3 start_texture = make_float3(device_lerp(bbox_min.x, bbox_max.x, uv_s.x),
                                               device_lerp(bbox_min.y, bbox_max.y, uv_s.y),
                                               height1);//(1.0f - min_hit.u - min_hit.v) * C_uv + min_hit.u * A_uv + min_hit.v * B_uv;


            float3 end_texture   = make_float3(device_lerp(bbox_min.x, bbox_max.x, uv_e.x),
                                               device_lerp(bbox_min.y, bbox_max.y, uv_e.y),
                                               height2);//(1.0f - max_hit.u - max_hit.v) * C2_uv + max_hit.u * A2_uv + max_hit.v * B2_uv;

            float len_texture;
            float3 new_dir = bvh_clamp_direction(normalize_len(end_texture - start_texture, &len_texture));

            start_texture -= offset_t * new_dir;
            geoframe.dir = dir;
            geoframe.P = P;
            geoframe.t_near = t_near;
            geoframe.t_far = t_far;

//            float l = len(max_hit.p - min_hit.p);
//            float l2 = (max_hit.t - min_hit.t) * len(dir);
//            if (l2 - l > EPS2) {
//                printf("%f %f diff %f\n", l2, l, l - l2);
//            }
//
//            float3 P_min = dir * min_hit.t + P;
//            float3 P_max = dir * max_hit.t + P;

            /*if (l2 - l > EPS2) {
                printf("%f %f diff %f\n", l2, l, l - l2);
            }*/


            /*if (min_hit.t < 0) {
                start_texture = start_texture + new_dir * (-min_hit.t / (max_hit.t - min_hit.t) * len_texture + EPS2);
                len_texture = len(start_texture - end_texture);
                min_hit.t = 0.0f;
            }*/
//            if (min_hit.t > 0) {
//                min_hit.t += EPS2;
//            } else {
//                min_hit.t -= EPS2;
//            }
//            max_hit.t += EPS2;

            float len_world = max_hit.t - min_hit.t;
            float mult = len_texture / len_world;
            if (ray_t != FLT_MAX) {
                ray_t = (ray_t - min_hit.t) * mult + offset_t;
            }

            geoframe.t_min = min_hit.t;
            geoframe.t_mult = mult;
            t_far = len_texture + offset_t + EPS;
            t_near = (min_hit.t > 0.0f) ? offset_t : (-min_hit.t) * mult + EPS + offset_t;

            dir = new_dir;
            idir = bvh_inverse_direction(dir);
            P = start_texture;
            return true;
        } else {
            printf("%f %f %f ignoring\n", min_hit.t, ray_t, max_hit.t);
        }
    }
    return false;
}