#pragma once
#define GEOPATTERN_SENTINEL ~0
#define INSTANCE_SENTINEL 0
#include "util/util_mat.h"
#include "../kernel_types.h"
#define  D_SD_OBJECT_NEGATIVE_SCALE_APPLIED (1 << 3)

const float EPS = 1e-6f;
const float EPS2 = 1e-4f;
const float shift_eps = 1e-3f;
const float offset_t = 1e-2f;

struct HitData {
    float t;
    bool hit;
    bool isUVWSet = false;
    //union {
    float3 p;
    float u, v, w;
    //};
};

struct GeopatternFrame {
    float3 dir;
    float3 P;
    float t_near;
    float t_far;
    float min_t;
    float mult_t;
};

ccl_device_inline void handle_hit(HitData &min, HitData &max, float3 P, float3 dir, float t, float u, float v, float3 a, float3 b, float3 c) {
    if (!((t < min.t) || (t > max.t))) {
        return;
    }

    float3 point = triangle_refine(P, dir, t, a, b, c);
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
//
//ccl_device_inline void barycentric(const float3 tri_a, const float3 tri_b, const float3 tri_c, const float3 p, float &u, float &v)
//{
//    float3 N = cross(tri_c - tri_b, tri_b - tri_a);
//    float A = len(N);
//    const float3 n = normalize(N);
//    u = len(cross(tri_c - tri_b, p - tri_b)) / A;
//    v = len(cross(tri_a - tri_c, p - tri_c)) / A;
//}

ccl_device_inline float sdistance_to_plane(const float3 tri_a, const float3 tri_b, const float3 tri_c, int object_flag,  const float3 p) {
    const float3 n = triangle_normal(tri_a, tri_b, tri_c, object_flag);
    return dot (n, p - tri_a);
}

ccl_device_inline float calculate_dot(const float3 tri_a, const float3 tri_b, const float3 tri_c,
                                      const float3 tri_n_a, const float3 tri_n_b, const float3 tri_n_c,
                                      int object_flag,
                                      const float3 point, const float w) {
    const float3 T_a = tri_a + w * tri_n_a;
    const float3 T_b = tri_b + w * tri_n_b;
    const float3 T_c = tri_c + w * tri_n_c;
    const float3 n = triangle_normal(T_a, T_b, T_c, object_flag);
    return dot(n, point - T_a);
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

ccl_device_inline float3 prism_point_uvw_coords(const float3 tri_a, const float3 tri_b, const float3 tri_c,
                                                const float3 tri_n_a, const float3 tri_n_b, const float3 tri_n_c,
                                                int object_flag,
                                                const float3 point, const float EPS) {
    float w = 0.0f;
    float min = 0.0f;
    float max = 1.0f;

    float f_min_dot = calculate_dot(tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c, object_flag, point, min);

    if ((std::abs(f_min_dot) <  EPS)) {
        return calculate_uvw(tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c, object_flag, point, min);
    }

    float f_max_dot = calculate_dot(tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c, object_flag, point, max);
    if ((std::abs(f_max_dot) <  EPS)) {
        return calculate_uvw(tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c, object_flag, point, max);
    }

    // usually 12 iterations are enough
    float f_mid_dot = 0.0f;
    for (int i = 0; i < 40; ++i) {
        w = 0.5f * min + 0.5f * max;

        f_mid_dot = calculate_dot(tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c, object_flag, point, w);

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

    if (f_mid_dot >= 10 * EPS) {
        printf("prism_point_uvw_coords: %f %f %f dot %f\n", (double)point.x, (double)point.y, (double)point.z, (double)f_mid_dot);
    }

    return calculate_uvw(tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c, object_flag, point, w);
}
