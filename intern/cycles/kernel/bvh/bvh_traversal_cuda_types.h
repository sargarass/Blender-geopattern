#pragma once

#define GEOPATTERN_SENTINEL_RAY (ENTRYPOINT_SENTINEL + 1)
#define GEOPATTERN_SENTINEL (ENTRYPOINT_SENTINEL + 2)
#define INSTANCE_SENTINEL  (ENTRYPOINT_SENTINEL + 3)
#define _MAX_SENTINEL (ENTRYPOINT_SENTINEL + 4)

#define IS_SENTINEL(x) ((x) >= ENTRYPOINT_SENTINEL && (x) < _MAX_SENTINEL)

#include "util/util_mat.h"
#include "../kernel_types.h"
#define  D_SD_OBJECT_NEGATIVE_SCALE_APPLIED (1 << 3)
//#define USE_TRIANGLE_MAPPING
ccl_device uint hash(uint kx, uint ky, uint kz);
//const float EPS = 1e-6f;
//const float EPS2 = 1e-4f;
//const float shift_eps = 1e-2f;
//const float offset_t = EPS2;
//const float offset_t2 = 1e-5f;

const float EPS = 1e-5f;
const float EPS2 = 1e-4f;
const float shift_eps = 1e-4f;
const float offset_t = 1e-4f;

enum class Side {
    Top,
    Down,
    AB,
    BC,
    CA,
    UVW,
    MVC
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

    union {
        struct {
            float u, v, w;
        };
        float mvc[6];
    };

    Side side;
};

struct HitData_3 {
    float t;
    float3 p;
    float u, v, w;
    Side side;
};

constexpr size_t ray_size = 1;

struct GeopatternFrame2 {
    float3 dir;
    float3 P;
    float t_near;
    float t_far;
    int object_nodes;
    int object;
    float height;
    Mat3 QQ;
    Mat3 Q;
    float2 A, B, C;
    float bbox_max_z, bbox_min_z;
    struct {
        int num_rays;
        int current_ray = -1;
        float3 dir[ray_size];
        float3 P[ray_size];
        float t_near[ray_size];
        float t_far[ray_size];
        float t_mult[ray_size];
        float t_min[ray_size];
    } rays;
};

struct GeopatternFrame {
    float3 dir;
    float3 P;
    float t_near;
    float t_far;
    float t_mult;
    float t_min;
    float len_texture;
    float len_world;

    float height;
    Mat3 QQ;
    Mat3 Q;
    float2 A, B, C;
    float bbox_max_z, bbox_min_z;
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


ccl_device_inline
uint64_t getGlobalIdx3DZ() {
    uint64_t blockId = blockIdx.x
                       + blockIdx.y * gridDim.x
                       + gridDim.x * gridDim.y * blockIdx.z;
    return blockId * blockDim.z + threadIdx.z;
}

ccl_device_inline
uint64_t getGlobalIdx3DZXY()
{
    uint64_t blockId = blockIdx.x
                       + blockIdx.y * gridDim.x
                       + gridDim.x * gridDim.y * blockIdx.z;
    return blockId * (blockDim.x * blockDim.y * blockDim.z)
           + (threadIdx.z * (blockDim.x * blockDim.y))
           + (threadIdx.y * blockDim.x)
           + threadIdx.x;
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
    if (fabs(1.0f - (u + v)) < EPS) {
        v = 1.0f - u;
    }
}

ccl_device_inline void barycentric_float2(const float2 tri_a, const float2 tri_b, const float2 tri_c, const float2 p, float &u, float &v)
{
    const float2 v0 = tri_a - tri_c, v1 = tri_b - tri_c, v2 = p - tri_c;
    const float d00 = dot(v0, v0);
    const float d01 = dot(v0, v1);
    const float d11 = dot(v1, v1);
    const float d20 = dot(v2, v0);
    const float d21 = dot(v2, v1);
    float invDenom = 1.0f / (d00 * d11 - d01 * d01);
    u = (d11 * d20 - d01 * d21) * invDenom;
    v = (d00 * d21 - d01 * d20) * invDenom;

    if (fabs(1.0f - (u + v)) < EPS) {
        v = 1.0f - u;
    }

}

template<class A, class B>
ccl_device_inline A device_lerp(const A& a, const A& b, const B& t)
{
    return (A)(a * ((B)1 - t) + b * t);
}

ccl_device_inline float3 my_ray_offset(float3 P, float3 Ng)
{
    const float epsilon_f = 1e-4f;
    return P + epsilon_f*Ng;
}

template <typename T>
ccl_device_inline float3 inverse_mapping(float3 tri_a, float3 tri_b, float3 tri_c, float3 tri_n_a, float3 tri_n_b, float3 tri_n_c, T &geoframe, float3 p) {
    float2 point = make_float2(p.x, p.y);
    float u, v;
    barycentric_float2(geoframe.A, geoframe.B, geoframe.C, point, u, v);
    float w = clamp((p.z - geoframe.bbox_min_z) / (geoframe.bbox_max_z - geoframe.bbox_min_z), 0.0f, 1.0f);
    const float3 T_a = device_lerp(tri_a, tri_a + tri_n_a, w);
    const float3 T_b = device_lerp(tri_b, tri_b + tri_n_b, w);
    const float3 T_c = device_lerp(tri_c, tri_c + tri_n_c, w);
    float3 worldPoint = ((1.0f - u - v) * T_c + u * T_a + v * T_b);
    return worldPoint;
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

ccl_device_inline void handle_hit_triangle(HitData_2 &min, HitData_2 &max, float3 P, float3 dir, const float3 a, const float3 b, const float3 c, const float t, const float u, const float v, const float w, const Side side) {
    if (!((t < min.t) || (t > max.t))) {
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
        min.side = side;
    }

    if (t > max.t) {
        max.hit = true;
        max.t = t;
        max.u = u;
        max.v = v;
        max.w = w;
        max.p = point;
        max.side = side;
    }
}

ccl_device_inline bool handle_hit_triangle_new(HitData_3 &data, Side side, const float3 a, const float3 b, const float3 c, const float t, const float u, const float v, const float w) {
    data.p = (1.0f - u - v) * c + u * a + v * b;
    data.u = u;
    data.v = v;
    data.w = w;
    data.t = t;
    data.side = side;
    return true;
}

ccl_device_forceinline float3 extend_borders(float3 P, float3 v1, float3 v2) {
    float3 e1 = (P - v1);
    float3 e2 = (P - v2);
    return P + normalize(e1 + e2) * EPS;
}

ccl_device_forceinline float2 extend_borders_float2(float2 P, float2 v1, float2 v2) {
    float2 e1 = normalize(P - v1);
    float2 e2 = normalize(P - v2);
    return P + (e1 + e2) * shift_eps;
}

ccl_device_forceinline float3 bilinear_interpolation(const float3 q00, const float3 q10, const float3 q11, const float3 q01, const float u, const float v) {
    return (1.0f - u)*(1.0f - v) * q00 + (1.0f - u)*v * q01 + u * (1.0f - v) * q10 + u*v* q11;
}

ccl_device_forceinline bool handle_hit_patch_new(HitData_3 &data, Side side,
                                        const float3 q00, const float3 q10, const float3 q11, const float3 q01,
                                        const float3 tri_a, const float3 tri_b, const float3 tri_c,
                                        const float3 tri_n_a, const float3 tri_n_b, const float3 tri_n_c,
                                        float t, float u, float v, Edge e) {
    float3 point = (1.0f - u)*(1.0f - v) * q00 + (1.0f - u)*v * q01 + u * (1.0f - v) * q10 + u*v* q11;
#ifdef USE_TRIANGLE_MAPPING
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
        printf("handle_hit_patch_new %f\n", d);
    }
#else
    data.u = u;
    data.v = v;
    data.w = 0.0f;
    data.p = point;
    data.t = t;
    data.side = side;
#endif
    return true;
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
    if (u >= 0.0f && v >= 0.0f && (u + v) <= 1.0f) {
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
    //float3 proj = point - dot(n, v0) * n;
    float u, v;
    barycentric(T_a, T_b, T_c, point, u, v);
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

template <typename T>
ccl_device_forceinline void swap(T &a, T &b) {
    T tmp = a;
    a = b;
    b = tmp;
}

ccl_device_forceinline float min_f(float a, float b)
{
    return (a < b) ? a : b;
}
ccl_device_forceinline float max_f(float a, float b)
{
    return (b < a) ? a : b;
}

ccl_device_forceinline float clamp_f(float val, float min, float max)
{
    return min_f(max_f(val, min), max);
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

    /*if (u1 <= (1.0f + EPS) && u1 >= -EPS) {
        u1 = clamp_f(u1, 0, 1.0f);
    }*/

    if (0.0f <= u1 && u1 <= 1.0f) {                  // is it inside the patch?
        float3 pa = device_lerp(q00, q10, u1);        // point on edge e10 (Fig. 4)
        float3 pb = device_lerp(e00, e11, u1);        // it is, actually, pb - pa
        float3 n = cross(dir, pb);
        det = dot(n, n);
        n = cross(n, pa);
        float t1 = dot(n, pb);
        float v1 = dot(n, dir);
        /*if (v1 <= det + EPS && v1 >= -EPS) {
            v1 = clamp_f(det, 0, 1.0f);
        }
*/
        if (0.0f <= v1 && v1 <= det) {  // if t1 > t,
            t = t1;
            *num_intersections = 1;
            *isect1_t = t1/det;
            *isect1_u = u1;
            *isect1_v = v1/det;
        }
    }

   /* if (u2 <= 1.0f + EPS && u2 >= -EPS) {
        u2 = clamp_f(u2, 0, 1.0f);
    }*/

    if (0.0f <= u2 && u2 <= 1.0f) {                   // it is slightly different,
        float3 pa = device_lerp(q00, q10, u2);        // since u1 might be good
        float3 pb = device_lerp(e00, e11, u2);        // and we need 0 < t2 < t1
        float3 n = cross(dir, pb);
        det = dot(n, n);
        n = cross(n, pa);
        float t2 = dot(n, pb)/det;
        float v2 = dot(n, dir);

        /*if (v2 <= 1.0f + EPS && v2 >= -EPS) {
            v2 = clamp_f(v2, 0, 1.0f);
        }*/

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

    /*if (std::abs(f_mid_dot) >= 10 * EPS) {
        printf("triangle %f %f %f | %f %f %f | %f %f %f\nnormals %f %f %f | %f %f %f | %f %f %f\nprism_point_uvw_coords: %f %f %f dot %f\n",
               tri_a.x, tri_a.y, tri_a.z,
               tri_b.x, tri_b.y, tri_b.z,
               tri_c.x, tri_c.y, tri_c.z,
               tri_n_a.x, tri_n_a.y, tri_n_a.z,
               tri_n_b.x, tri_n_b.y, tri_n_b.z,
               tri_n_c.x, tri_n_c.y, tri_n_c.z,
               (double)point.x, (double)point.y, (double)point.z, (double)f_mid_dot);
    }*/

    return calculate_uvw(tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c, object_flag, point, w);
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
#ifdef USE_TRIANGLE_MAPPING
    float3 uvw = prism_point_uvw_coords(tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c, 0, point, EPS);
    const float w = uvw.z;

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

    if (!((u + v) <= 1.0f && u >= 0.0f && v >= 0.0f)) {
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
        printf("handle_hit_patch %f\n", d);
    }
#else
    if (t < min.t) {
        min.hit = true;
        min.t = t;
        min.u = u;
        min.v = v;
        min.w = 0.0f;
        min.p = point;
        min.side = (Edge::AB == e)? Side::AB : ((Edge::BC == e)? Side::BC : Side::CA);
    }

    if (t > max.t) {
        max.hit = true;
        max.t = t;
        max.u = u;
        max.v = v;
        max.w = 0.0f;
        max.p = point;
        max.side = (Edge::AB == e)? Side::AB : ((Edge::BC == e)? Side::BC : Side::CA);
    }
#endif
}

ccl_device_forceinline float norm(float3 v) {
    return sqrtf(dot(v, v));
}

ccl_device_forceinline float sqrnorm(float3 v) {
    return dot(v, v);
}

ccl_device_forceinline bool computeCoordinates(
        float3 eta,
float3 tri_a,
        float3 tri_b,
float3 tri_c,
        float3 tri_n_a,
float3 tri_n_b,
        float3 tri_n_c,
float (& weights)[6])
{
float w_weights[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
float3 tri_a_2 = tri_a + tri_n_a;
float3 tri_b_2 = tri_b + tri_n_b;
float3 tri_c_2 = tri_c + tri_n_c;

float3 const cage_vertices[6] {tri_a, tri_b, tri_c, tri_a_2, tri_b_2, tri_c_2};
//    const unsigned int cage_triangles[2][3] = {{0, 1, 2}, {3, 4, 5}};
const unsigned int cage_triangles[8][3] = {
        {0, 1, 2},
        {3, 4, 5},
        {0, 1, 3},
        {3, 1, 4},
        {1, 2, 4},
        {4, 2, 5},
        {2, 0, 5},
        {5, 0, 3},
};

const unsigned int cage_quads[3][4] = {{2, 0, 3, 5}, {0, 1, 4, 3}, {1, 2, 5, 4}};
float epsilon = 1e-5f;

const unsigned int n_vertices = 6;
const unsigned int n_triangles = 8;
const unsigned int n_quads = 3;

for (int i = 0; i < 6; ++i) {
weights[i] = 0.0f;
w_weights[i] = 0.0f;
}

float sumWeights = 0.0f;

float d[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
float3 u[6];

for( unsigned int v = 0 ; v < 6; ++v )
{
d[ v ] = norm( eta - cage_vertices[ v ] );
if( d[ v ] < epsilon )
{
weights[v] = 1.0;
return true;
}
u[ v ] = ( cage_vertices[v] - eta ) / d[v];
}

// CAGE TRIANGLES:
{
unsigned int vid[3];
float l[3]; float theta[3] ; float w[3];

for( unsigned int t = 0 ; t < n_triangles ; ++t )
{
// the Norm is CCW :
for( unsigned int i = 0 ; i <= 2 ; ++i )
vid[i] =  cage_triangles[t][i];

for( unsigned int i = 0 ; i <= 2 ; ++i ) {
l[ i ] = norm( u[ vid[ ( i + 1 ) % 3 ] ] - u[ vid[ ( i + 2 ) % 3 ] ] );
}

for( unsigned int i = 0 ; i <= 2 ; ++i )
theta[i] = 2.0f * asin( l[i] / 2.0f );

float determinant = dot( cage_vertices[vid[0]] - eta , cross( cage_vertices[vid[1]] - cage_vertices[vid[0]] , cage_vertices[vid[2]] - cage_vertices[vid[0]] ) );
float sqrdist = determinant*determinant / (4 * sqrnorm(cross( cage_vertices[vid[1]] - cage_vertices[vid[0]] , cage_vertices[vid[2]] - cage_vertices[vid[0]] )) );
float dist = sqrtf( sqrdist );

if( dist < epsilon ) {
// then the point eta lies on the support plane of the triangle
float h = ( theta[0] + theta[1] + theta[2] ) / 2.0f;
if( M_PIf32 - h < epsilon ) {
float u, v;
barycentric(cage_vertices[vid[0]], cage_vertices[vid[1]], cage_vertices[vid[2]], eta, u, v);
weights[ vid[0] ] = u;
weights[ vid[1] ] = v;
weights[ vid[2] ] = 1 - u - v;
return true;
}
}

float3 pt[3] , N[3];

for( unsigned int i = 0 ; i < 3 ; ++i )
pt[i] = cage_vertices[ vid[i] ];

for( unsigned int i = 0 ; i < 3 ; ++i )
N[i] = cross( pt[(i+1)%3] - eta , pt[(i+2)%3] - eta );

for( unsigned int i = 0 ; i <= 2 ; ++i ) {
w[i] = 0.0;
for( unsigned int j = 0 ; j <= 2 ; ++j )
w[i] += theta[j] * dot( N[i] , N[j] ) / ( 2.0f * norm(N[j]) );

w[i] /= determinant;
}

sumWeights += ( w[0] + w[1] + w[2] );
w_weights[ vid[0] ] += w[0];
w_weights[ vid[1] ] += w[1];
w_weights[ vid[2] ] += w[2];
}
}

    for( unsigned int v = 0 ; v < n_vertices ; ++v )
        weights[v]  = w_weights[v] / sumWeights;

    return false;
}

ccl_device_forceinline bool common_part_geopattern_inside(
        KernelGlobals *kg,
        const int prim_addr,
        const int object_idx,
        const uint geopattern_link,
        const uint geopattern_clipbox_link,
        const float geopatten_height,
        const uint geopattern_flags,
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

//    object_normal_transform(kg, geopattern_link, &tri_n_a);
//    object_normal_transform(kg, geopattern_link, &tri_n_b);
//    object_normal_transform(kg, geopattern_link, &tri_n_c);

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

//    min_hit.hit = false;
//    min_hit.t = FLT_MAX;
//
//    max_hit.hit = false;
//    max_hit.t = -FLT_MAX;
    float u, v, t;

    HitData_2 min_hit;
    min_hit.hit = false;
    min_hit.t = FLT_MAX;

    HitData_2 max_hit;
    max_hit.hit = false;
    max_hit.t = -FLT_MAX;

//    if (ray_triangle_intersect(P, dir, ray_t, prism_tri_1_a, prism_tri_1_b, prism_tri_1_c, &u, &v, &t)) {
//        isect->type = PRIMITIVE_TRIANGLE;
//        isect->u = 0.5;
//        isect->v = 0.5;
//        isect->t = t;
//        isect->prim = prim_addr;
//        isect->object = object;
//        isect->entry_object = GEOPATTERN_NO_LINK;
//        isect->entry_prim = GEOPATTERN_NO_LINK;
//    }

    int total_intersection = 0;

    if (ray_triangle_intersect_allow_backtracing(P, dir, prism_tri_1_a, prism_tri_1_b, prism_tri_1_c, &u, &v, &t)) {
        total_intersection++;

#ifndef RENDER_BBOX
        if (geopattern_flags & GEOPATTERN_FLAGS_RENDERBASE) {
#endif
            if (t < isect->t && t > 0.0f) {
                isect->type = PRIMITIVE_TRIANGLE;
                isect->u = u;
                isect->v = v;
                isect->t = t;
                isect->prim = prim_addr;
                isect->object = object;
                isect->entry_object = GEOPATTERN_NO_LINK;
                isect->entry_prim = GEOPATTERN_NO_LINK;
            }
#ifndef RENDER_BBOX
        }
#endif

        handle_hit_triangle(min_hit, max_hit, P, dir, prism_tri_1_a, prism_tri_1_b, prism_tri_1_c, t, u, v, 0.0f, Side::Down);
    }

    if (ray_triangle_intersect_allow_backtracing(P, dir, prism_tri_2_a, prism_tri_2_b, prism_tri_2_c, &u, &v, &t)) {
        total_intersection++;
        handle_hit_triangle(min_hit, max_hit, P, dir, prism_tri_2_a, prism_tri_2_b, prism_tri_2_c, t, u, v, 1.0f, Side::Top);
#ifdef RENDER_BBOX
        if (t < isect->t && t > 0.0f) {
            isect->type = PRIMITIVE_TRIANGLE;
            isect->u = u;
            isect->v = v;
            isect->t = t;
            isect->prim = prim_addr;
            isect->object = object;
            isect->entry_object = GEOPATTERN_NO_LINK;
            isect->entry_prim = GEOPATTERN_NO_LINK;
        }
#endif
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
#ifdef RENDER_BBOX
        if (t < isect->t && t > 0.0f) {
            isect->type = PRIMITIVE_TRIANGLE;
            isect->u = u;
            isect->v = v;
            isect->t = t;
            isect->prim = prim_addr;
            isect->object = object;
            isect->entry_object = GEOPATTERN_NO_LINK;
            isect->entry_prim = GEOPATTERN_NO_LINK;
        }
#endif

        if (num_intersection > 1) {
            total_intersection++;
            handle_hit_patch(min_hit, max_hit, P, dir,
                                 prism_tri_1_a, prism_tri_1_b, prism_tri_2_b, prism_tri_2_a,
                                 tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
                                 t2, u2, v2, Edge::AB);
#ifdef RENDER_BBOX
            if (t2 < isect->t && t2 > 0.0f) {
                isect->type = PRIMITIVE_TRIANGLE;
                isect->u = u2;
                isect->v = v2;
                isect->t = t2;
                isect->prim = prim_addr;
                isect->object = object;
                isect->entry_object = GEOPATTERN_NO_LINK;
                isect->entry_prim = GEOPATTERN_NO_LINK;
            }
#endif
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
#ifdef RENDER_BBOX
        if (t < isect->t && t > 0.0f) {
            isect->type = PRIMITIVE_TRIANGLE;
            isect->u = u;
            isect->v = v;
            isect->t = t;
            isect->prim = prim_addr;
            isect->object = object;
            isect->entry_object = GEOPATTERN_NO_LINK;
            isect->entry_prim = GEOPATTERN_NO_LINK;
        }
#endif


        if (num_intersection > 1) {
            total_intersection++;
            handle_hit_patch(min_hit, max_hit, P, dir,
                             prism_tri_1_b, prism_tri_1_c, prism_tri_2_c, prism_tri_2_b,
                             tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
                             t2, u2, v2, Edge::BC);
#ifdef RENDER_BBOX
            if (t2 < isect->t && t2 > 0.0f) {
                isect->type = PRIMITIVE_TRIANGLE;
                isect->u = u2;
                isect->v = v2;
                isect->t = t2;
                isect->prim = prim_addr;
                isect->object = object;
                isect->entry_object = GEOPATTERN_NO_LINK;
                isect->entry_prim = GEOPATTERN_NO_LINK;
            }
#endif
        }
    }

    num_intersection = 0;
    if (ray_patch_intersection_allow_backtracing(P, dir, prism_tri_1_c, prism_tri_1_a, prism_tri_2_a, prism_tri_2_c, &u, &v, &t, &u2, &v2, &t2, &num_intersection)) {
        total_intersection++;
        handle_hit_patch(min_hit, max_hit, P, dir,
                         prism_tri_1_c, prism_tri_1_a, prism_tri_2_a, prism_tri_2_c,
                         tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
                         t, u, v, Edge::CA);
#ifdef RENDER_BBOX
        if (t < isect->t && t > 0.0f) {
            isect->type = PRIMITIVE_TRIANGLE;
            isect->u = u;
            isect->v = v;
            isect->t = t;
            isect->prim = prim_addr;
            isect->object = object;
            isect->entry_object = GEOPATTERN_NO_LINK;
            isect->entry_prim = GEOPATTERN_NO_LINK;
        }
#endif
        if (num_intersection > 1) {
            total_intersection++;
            handle_hit_patch(min_hit, max_hit, P, dir,
                             prism_tri_1_c, prism_tri_1_a, prism_tri_2_a, prism_tri_2_c,
                             tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
                             t2, u2, v2, Edge::CA);
#ifdef RENDER_BBOX
            if (t2 < isect->t && t2 > 0.0f) {
                isect->type = PRIMITIVE_TRIANGLE;
                isect->u = u2;
                isect->v = v2;
                isect->t = t2;
                isect->prim = prim_addr;
                isect->object = object;
                isect->entry_object = GEOPATTERN_NO_LINK;
                isect->entry_prim = GEOPATTERN_NO_LINK;
            }
#endif
        }
    }

    //if (total_intersection > 2) {
//        min_hit = hit[0];
//        max_hit = hit[total_intersection - 1];

    //    printf("num_intersections %d\n", total_intersection);
   // }
//

//
//
//
#ifdef RENDER_BBOX
    return false;
#endif
    if (min_hit.hit && max_hit.hit
        && fabs(min_hit.t - max_hit.t) > EPS
        && ((min_hit.t > 0 && ray_t > min_hit.t)
            || (min_hit.t < 0.0f && max_hit.t > 0.0f))) {

//        bool isec = false;
//
//        if (min_hit.hit && min_hit.t > 0 && isect->t > min_hit.t) {
//            isec = true;
//            t = min_hit.t;
//            u = min_hit.u;
//            v = min_hit.v;
//        } else {
//            if (max_hit.hit && max_hit.t > 0 && isect->t > max_hit.t) {
//                isec = true;
//                t = max_hit.t;
//                u = max_hit.u;
//                v = max_hit.v;
//            }
//        }
//
//        if (isec) {
//            isect->type = PRIMITIVE_TRIANGLE;
//            isect->u = u;
//            isect->v = v;
//            isect->t = t;
//            isect->prim = prim_addr;
//            isect->object = object;
//            isect->entry_object = GEOPATTERN_NO_LINK;
//            isect->entry_prim = GEOPATTERN_NO_LINK;
//        }
//        return false;
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
            float3 uvw = prism_point_uvw_coords(tri_a, tri_b, tri_c,
                    tri_n_a, tri_n_b, tri_n_c,
                    object_flag, P, EPS);
            if (!((uvw.x + uvw.y) <= 1.0f && uvw.x >= 0.0f  && uvw.y >= 0.0f)) {
                go_inside = false;
            }

            min_hit.p = P;
            min_hit.t = 0.0f;
            min_hit.u = uvw.x;
            min_hit.v = uvw.y;
            min_hit.w = uvw.z;
            min_hit.side = Side::UVW;


//            min_hit.p = P;
//            min_hit.t = 0.0f;
//            min_hit.side = Side::MVC;
//            computeCoordinates(P, tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c, min_hit.mvc);

//            const float3 T_a = device_lerp(tri_a, tri_a + tri_n_a, min_hit.w);
//            const float3 T_b = device_lerp(tri_b, tri_b + tri_n_b, min_hit.w);
//            const float3 T_c = device_lerp(tri_c, tri_c + tri_n_c, min_hit.w);
//            float d = sdistance_to_plane(T_a, T_b, T_c, 0, P);
//            if (std::abs(d) > 1e-4f) {
//                printf("%f\n", d);
//            }
        }
//        else {
//            min_hit.side = Side::MVC;
//            computeCoordinates(min_hit.p, tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c, min_hit.mvc);
//        }
//
//        max_hit.side = Side::MVC;
//        computeCoordinates(max_hit.p, tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c, max_hit.mvc);
//
//        float3 pp = tri_a * max_hit.mvc[0] + tri_b * max_hit.mvc[1] + tri_c * max_hit.mvc[2]
//        + ((tri_a + tri_n_a) * max_hit.mvc[3])+ ((tri_b + tri_n_b) * max_hit.mvc[4])+ ((tri_c + tri_n_c) * max_hit.mvc[5]);
//
//        float d = len(pp - max_hit.p);
//        if (d > 0.1) {
//            printf("float3 a = make_float3(__int_as_float(%d), __int_as_float(%d), __int_as_float(%d));\n"
//                   "float3 b = make_float3(__int_as_float(%d), __int_as_float(%d), __int_as_float(%d));\n"
//                   "float3 c = make_float3(__int_as_float(%d), __int_as_float(%d), __int_as_float(%d));\n"
//                   "float3 a_n = make_float3(__int_as_float(%d), __int_as_float(%d), __int_as_float(%d));\n"
//                   "float3 b_n = make_float3(__int_as_float(%d), __int_as_float(%d), __int_as_float(%d));\n"
//                   "float3 c_n = make_float3(__int_as_float(%d), __int_as_float(%d), __int_as_float(%d));\n"
//                   "float3 p = make_float3(__int_as_float(%d), __int_as_float(%d), __int_as_float(%d)); \ndist%f\n\n",
//                   __float_as_int(tri_a.x), __float_as_int(tri_a.y), __float_as_int(tri_a.z),
//                   __float_as_int(tri_b.x), __float_as_int(tri_b.y), __float_as_int(tri_b.z),
//                   __float_as_int(tri_c.x), __float_as_int(tri_c.y), __float_as_int(tri_c.z),
//                   __float_as_int(tri_n_a.x), __float_as_int(tri_n_a.y), __float_as_int(tri_n_a.z),
//                   __float_as_int(tri_n_b.x), __float_as_int(tri_n_b.y), __float_as_int(tri_n_b.z),
//                   __float_as_int(tri_n_c.x), __float_as_int(tri_n_c.y), __float_as_int(tri_n_c.z),
//                   __float_as_int(max_hit.p.x), __float_as_int(max_hit.p.y), __float_as_int(max_hit.p.z), d);
//        }

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
            float3 object_bbox_min = {
                    min4(xx.x, xx.y, xx.z, xx.w),
                    min4(yy.x, yy.y, yy.z, yy.w),
                    min4(zz.x, zz.y, zz.z, zz.w)// - shift_eps
            };

            float3 object_bbox_max = {
                    max4(xx.x, xx.y, xx.z, xx.w),
                    max4(yy.x, yy.y, yy.z, yy.w),
                    max4(zz.x, zz.y, zz.z, zz.w)
            };

            if (object_bbox_max.z == object_bbox_min.z) {
                object_bbox_max.z += geopatten_height - shift_eps;
                object_bbox_min.z -= shift_eps;
            } else {
//            bbox_min.x -= shift_eps;
//            bbox_max.x += shift_eps;
//            bbox_min.y -= shift_eps;
//            bbox_max.y += shift_eps;
              object_bbox_min.z -= shift_eps;
              object_bbox_max.z += shift_eps;
            }

            float3 clipbox_bmin = float4_to_float3(kernel_tex_fetch(__geopattern_clipbox, geopattern_clipbox_link));
            float3 clipbox_bmax = float4_to_float3(kernel_tex_fetch(__geopattern_clipbox, geopattern_clipbox_link + 1));

            float x_size = object_bbox_max.x - object_bbox_min.x;
            float y_size = object_bbox_max.y - object_bbox_min.y;
            float z_size = object_bbox_max.z - object_bbox_min.z;

            float3 bbox_min;
            float3 bbox_max;
            if (geopattern_flags & GEOPATTERN_FLAGS_RANDOM_OFFSET) {
//                hash()
//                #define MIX1(a) (((214013 * (a) + 2531011) >> 16)&0x7FFF)
//                #define MIX(a, b) MIX1(MIX1(a) + MIX1(b))
//
//                float offset_xx = MIX(MIX(MIX(__float_as_int(tri_a.x), prim_addr), __float_as_int(tri_b.x)), __float_as_int(tri_c.x)) / 32767.0f;
//                float offset_x = offset_xx * 0.1f;
//                float offset_yy = MIX(MIX(MIX(__float_as_int(tri_a.y), prim_addr), __float_as_int(tri_b.y)), __float_as_int(tri_c.y)) / 32767.0f;
//                float offset_y = offset_yy * 0.1f;


                float offset_x = ((float)hash(__float_as_int(tri_a.x), __float_as_int(tri_b.x), __float_as_int(tri_c.x)) / UINT32_MAX);
                float offset_y = ((float)hash(__float_as_int(tri_a.y), __float_as_int(tri_b.y), __float_as_int(tri_c.y)) / UINT32_MAX);

                float x_size = (clipbox_bmax.x - clipbox_bmin.x);
                float y_size = (clipbox_bmax.y - clipbox_bmin.y);

                clipbox_bmin.x += offset_x * 0.25 * x_size;
                clipbox_bmin.y += offset_y * 0.25 * y_size;
                clipbox_bmax.x -= offset_y * 0.25 * x_size;
                clipbox_bmax.y -= offset_x * 0.25 * y_size;

                if (clipbox_bmin.x > clipbox_bmax.x) {
                    swap(clipbox_bmax.x, clipbox_bmin.x);
                }
                if (clipbox_bmin.y > clipbox_bmax.y) {
                    swap(clipbox_bmax.y, clipbox_bmin.y);
                }
            }

            bbox_min.x = (clipbox_bmin.x * x_size) + object_bbox_min.x;
            bbox_min.y = (clipbox_bmin.y * y_size) + object_bbox_min.y;
            bbox_min.z = (clipbox_bmin.z * z_size) + object_bbox_min.z;

            bbox_max.x = (clipbox_bmax.x * x_size) + object_bbox_min.x;
            bbox_max.y = (clipbox_bmax.y * y_size) + object_bbox_min.y;
            bbox_max.z = (clipbox_bmax.z * z_size) + object_bbox_min.z;

            float2 tri_a_uv = kernel_tex_fetch(__prim_tri_uv_geopattern, tri_vindex.w + 0);
            float2 tri_b_uv = kernel_tex_fetch(__prim_tri_uv_geopattern, tri_vindex.w + 1);
            float2 tri_c_uv = kernel_tex_fetch(__prim_tri_uv_geopattern, tri_vindex.w + 2);

            tri_a_uv.x = clamp_f(tri_a_uv.x, 0.0, 1.0f);
            tri_b_uv.x = clamp_f(tri_b_uv.x, 0.0, 1.0f);
            tri_c_uv.x = clamp_f(tri_c_uv.x, 0.0, 1.0f);
            tri_a_uv.y = clamp_f(tri_a_uv.y, 0.0, 1.0f);
            tri_b_uv.y = clamp_f(tri_b_uv.y, 0.0, 1.0f);
            tri_c_uv.y = clamp_f(tri_c_uv.y, 0.0, 1.0f);

            float2 A_uv = make_float2(device_lerp(bbox_min.x, bbox_max.x, tri_a_uv.x),
                                      device_lerp(bbox_min.y, bbox_max.y, tri_a_uv.y));

            float2 B_uv = make_float2(device_lerp(bbox_min.x, bbox_max.x, tri_b_uv.x),
                                      device_lerp(bbox_min.y, bbox_max.y, tri_b_uv.y));

            float2 C_uv = make_float2(device_lerp(bbox_min.x, bbox_max.x, tri_c_uv.x),
                                      device_lerp(bbox_min.y, bbox_max.y, tri_c_uv.y));

            geoframe.A = A_uv;
            geoframe.B = B_uv;
            geoframe.C = C_uv;

            extend_borders_float2(geoframe.A, geoframe.B, geoframe.C);
            extend_borders_float2(geoframe.B, geoframe.A, geoframe.C);
            extend_borders_float2(geoframe.C, geoframe.B, geoframe.C);

            geoframe.height = geopatten_height;
            geoframe.bbox_max_z = bbox_max.z;
            geoframe.bbox_min_z = bbox_min.z;

            if (object_flag & D_SD_OBJECT_NEGATIVE_SCALE_APPLIED) {
                geoframe.Q[0][0] = (tri_c - tri_a).x;
                geoframe.Q[1][0] = (tri_c - tri_a).y;
                geoframe.Q[2][0] = (tri_c - tri_a).z;

                geoframe.Q[0][1] = (tri_b - tri_a).x;
                geoframe.Q[1][1] = (tri_b - tri_a).y;
                geoframe.Q[2][1] = (tri_b - tri_a).z;

            } else {
                geoframe.Q[0][0] = (tri_b - tri_a).x;
                geoframe.Q[1][0] = (tri_b - tri_a).y;
                geoframe.Q[2][0] = (tri_b - tri_a).z;

                geoframe.Q[0][1] = (tri_c - tri_a).x;
                geoframe.Q[1][1] = (tri_c - tri_a).y;
                geoframe.Q[2][1] = (tri_c - tri_a).z;
            }

            float3 Ng = geopatten_height * normalize((tri_n_a + tri_n_b + tri_n_c) / 3.0f);
            geoframe.Q[0][2] = Ng.x;
            geoframe.Q[1][2] = Ng.y;
            geoframe.Q[2][2] = Ng.z;

            float2 e1 = (B_uv - A_uv);
            float2 e2 = (C_uv - A_uv);

            geoframe.QQ[0][0] = e1.x;
            geoframe.QQ[1][0] = e1.y;
            geoframe.QQ[2][0] = 0;

            geoframe.QQ[0][1] = e2.x;
            geoframe.QQ[1][1] = e2.y;
            geoframe.QQ[2][1] = 0;

            geoframe.QQ[0][2] = 0;
            geoframe.QQ[1][2] = 0;
            geoframe.QQ[2][2] = ((bbox_max.z - bbox_min.z) > 0.0)? (bbox_max.z - bbox_min.z) : 1.0f;

#ifndef USE_TRIANGLE_MAPPING
            float3 _A_uv = make_float3(device_lerp(bbox_min.x, bbox_max.x, tri_a_uv.x),
                                       device_lerp(bbox_min.y, bbox_max.y, tri_a_uv.y),
                                       bbox_min.z);

            float3 _B_uv = make_float3(device_lerp(bbox_min.x, bbox_max.x, tri_b_uv.x),
                                       device_lerp(bbox_min.y, bbox_max.y, tri_b_uv.y),
                                       bbox_min.z);

            float3 _C_uv = make_float3(device_lerp(bbox_min.x, bbox_max.x, tri_c_uv.x),
                                       device_lerp(bbox_min.y, bbox_max.y, tri_c_uv.y),
                                       bbox_min.z);

            /*extend_borders(_A_uv, _B_uv, _C_uv);
            extend_borders(_B_uv, _A_uv, _C_uv);
            extend_borders(_C_uv, _A_uv, _B_uv);*/

            float3 _A_uv2 = make_float3(_A_uv.x,
                                        _A_uv.y,
                                        bbox_max.z);

            float3 _B_uv2 = make_float3(_B_uv.x,
                                        _B_uv.y,
                                        bbox_max.z);

            float3 _C_uv2 = make_float3(_C_uv.x,
                                        _C_uv.y,
                                        bbox_max.z);

            float3 start_texture;
            float3 end_texture;
            switch (min_hit.side) {
                case Side::Top: {
                    start_texture = (1.0f - min_hit.u - min_hit.v) * _C_uv2 + min_hit.u * _A_uv2 + min_hit.v * _B_uv2;
                    break;
                }
                case Side::Down: {
                    start_texture = (1.0f - min_hit.u - min_hit.v) * _C_uv + min_hit.u * _A_uv + min_hit.v * _B_uv;
                    break;
                }
                case Side::AB: {
                    start_texture = bilinear_interpolation(_A_uv, _B_uv, _B_uv2, _A_uv2, min_hit.u, min_hit.v);
                    break;
                }
                case Side::BC: {
                    start_texture = bilinear_interpolation(_B_uv, _C_uv, _C_uv2, _B_uv2, min_hit.u, min_hit.v);
                    break;
                }
                case Side::CA: {
                    start_texture = bilinear_interpolation(_C_uv, _A_uv, _A_uv2, _C_uv2, min_hit.u, min_hit.v);
                    break;
                }
                case Side::UVW: {
                    float2 uv_s = (1.0f - min_hit.u - min_hit.v) * tri_c_uv + min_hit.u * tri_a_uv + min_hit.v * tri_b_uv;
                    start_texture = make_float3(device_lerp(bbox_min.x, bbox_max.x, uv_s.x),
                                                device_lerp(bbox_min.y, bbox_max.y, uv_s.y),
                                                device_lerp(bbox_min.z, bbox_max.z, min_hit.w));
                    break;
                }
                case Side::MVC: {
                    start_texture = _A_uv * min_hit.mvc[0] + _B_uv * min_hit.mvc[1] + _C_uv * min_hit.mvc[2]
                                    + _A_uv2 * min_hit.mvc[3] + _B_uv2 * min_hit.mvc[4] + _C_uv2 * min_hit.mvc[5];
                    break;
                }
            }

            switch (max_hit.side) {
                case Side::Top: {
                    end_texture = (1.0f - max_hit.u - max_hit.v) * _C_uv2 + max_hit.u * _A_uv2 + max_hit.v * _B_uv2;
                    break;
                }
                case Side::Down: {
                    end_texture = (1.0f - max_hit.u - max_hit.v) * _C_uv + max_hit.u * _A_uv + max_hit.v * _B_uv;
                    break;
                }
                case Side::AB: {
                    end_texture = bilinear_interpolation(_A_uv, _B_uv, _B_uv2, _A_uv2, max_hit.u, max_hit.v);
                    break;
                }
                case Side::BC: {
                    end_texture = bilinear_interpolation(_B_uv, _C_uv, _C_uv2, _B_uv2, max_hit.u, max_hit.v);
                    break;
                }
                case Side::CA: {
                    end_texture = bilinear_interpolation(_C_uv, _A_uv, _A_uv2, _C_uv2, max_hit.u, max_hit.v);
                    break;
                }
                case Side::MVC: {
                    end_texture = _A_uv * max_hit.mvc[0] + _B_uv * max_hit.mvc[1] + _C_uv * max_hit.mvc[2]
                                    + _A_uv2 * max_hit.mvc[3] + _B_uv2 * max_hit.mvc[4] + _C_uv2 * max_hit.mvc[5];
                    break;
                }
            }
#else
            float2 uv_s = (1.0f - min_hit.u - min_hit.v) * tri_c_uv + min_hit.u * tri_a_uv + min_hit.v * tri_b_uv;
            float2 uv_e = (1.0f - max_hit.u - max_hit.v) * tri_c_uv + max_hit.u * tri_a_uv + max_hit.v * tri_b_uv;
            float3 start_texture = make_float3(device_lerp(bbox_min.x, bbox_max.x, uv_s.x),
                                               device_lerp(bbox_min.y, bbox_max.y, uv_s.y),
                                               device_lerp(bbox_min.z, bbox_max.z, min_hit.w));


            float3 end_texture   = make_float3(device_lerp(bbox_min.x, bbox_max.x, uv_e.x),
                                               device_lerp(bbox_min.y, bbox_max.y, uv_e.y),
                                               device_lerp(bbox_min.z, bbox_max.z, max_hit.w));
#endif

            float len_texture;
            float3 new_dir = bvh_clamp_direction(normalize_len(end_texture - start_texture, &len_texture));

            start_texture -= offset_t * new_dir;
            geoframe.dir = dir;
            geoframe.P = P;
            geoframe.t_near = t_near;
            geoframe.t_far = t_far;

            float len_world = max_hit.t - min_hit.t;
            float mult = len_texture / len_world;
            geoframe.len_texture = len_texture;
            geoframe.len_world = len_world;
            if (mult <= 0.0f) {
                mult = 1.0f;
            }


            if (ray_t != FLT_MAX) {
                ray_t = (ray_t - min_hit.t) * mult + offset_t;
            }

            geoframe.t_min = min_hit.t;
            geoframe.t_mult = mult;
            //t_far = len_texture + offset_t + EPS2 * len_texture;
            t_far = len_texture + offset_t + EPS2;

            float t_min_uv = (-min_hit.t) * mult;
            t_near = (min_hit.t >= 0.0f) ? offset_t: t_min_uv + offset_t;

            dir = new_dir;
            idir = bvh_inverse_direction(dir);
            P = start_texture;
            return true;
        }/* else {
            printf("%f %f %f %d %d ignoring!!!\n", min_hit.t, ray_t, max_hit.t, max_hit.side, max_hit.hit);
        }*/
    }
    return false;
}

ccl_device_forceinline bool common_part_geopattern_inside_many_rays(
        const int prim_addr,
        const int object_idx,
        const uint geopattern_link,
        const uint geopattern_clipbox_link,
        const float geopatten_height,
        const uint geopattern_flags,
        const float3 P,
        const float3 dir,
        const float t_near,
        const float t_far,
        const int object,
        int &stack_ptr,
        int *traversal_stack,
        int &node_addr,
        GeopatternFrame2 &geoframe,
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

    float u, v, t;

    int total_intersection = 0;

    if (ray_triangle_intersect_allow_backtracing(P, dir, prism_tri_1_a, prism_tri_1_b, prism_tri_1_c, &u, &v, &t)) {//
        if (geopattern_flags & GEOPATTERN_FLAGS_RENDERBASE) {
            if (t < isect->t && t > 0.0f) {
                isect->type = PRIMITIVE_TRIANGLE;
                isect->u = u;
                isect->v = v;
                isect->t = t;
                isect->prim = prim_addr;
                isect->object = object;
                isect->entry_object = GEOPATTERN_NO_LINK;
                isect->entry_prim = GEOPATTERN_NO_LINK;
            }
        }

        if (handle_hit_triangle_new(hit[total_intersection], Side::Down, prism_tri_1_a, prism_tri_1_b, prism_tri_1_c, t, u, v, 0.0f)) {
            total_intersection++;
        }
    }

    if (ray_triangle_intersect_allow_backtracing(P, dir, prism_tri_2_a, prism_tri_2_b, prism_tri_2_c, &u, &v, &t)) {//
        if (handle_hit_triangle_new(hit[total_intersection], Side::Top, prism_tri_2_a, prism_tri_2_b, prism_tri_2_c, t, u, v, 1.0f)) {
            total_intersection++;
        }
    }

    // side a, b
    int num_intersection = 0;
    float u2 = FLT_MAX, v2 = FLT_MAX, t2 = FLT_MAX;
    if (ray_patch_intersection_allow_backtracing(P, dir, prism_tri_1_a, prism_tri_1_b, prism_tri_2_b, prism_tri_2_a, &u, &v, &t, &u2, &v2, &t2, &num_intersection)) {
        if (handle_hit_patch_new(hit[total_intersection], Side::AB,
                         prism_tri_1_a, prism_tri_1_b, prism_tri_2_b, prism_tri_2_a,
                         tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
                         t, u, v, Edge::AB)) {
            total_intersection++;
        }

        if (   num_intersection > 1
            && handle_hit_patch_new(hit[total_intersection], Side::AB,
                             prism_tri_1_a, prism_tri_1_b, prism_tri_2_b, prism_tri_2_a,
                             tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
                             t2, u2, v2, Edge::AB)) {
            total_intersection++;
        }
    }

    // side b, c
    num_intersection = 0;
    if (ray_patch_intersection_allow_backtracing(P, dir, prism_tri_1_b, prism_tri_1_c, prism_tri_2_c, prism_tri_2_b, &u, &v, &t, &u2, &v2, &t2, &num_intersection)) {
        if (handle_hit_patch_new(hit[total_intersection], Side::BC,
                         prism_tri_1_b, prism_tri_1_c, prism_tri_2_c, prism_tri_2_b,
                         tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
                         t, u, v, Edge::BC)) {
            total_intersection++;
        }

        if (   num_intersection > 1
            && handle_hit_patch_new(hit[total_intersection], Side::BC,
                             prism_tri_1_b, prism_tri_1_c, prism_tri_2_c, prism_tri_2_b,
                             tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
                             t2, u2, v2, Edge::BC)) {
            total_intersection++;
        }
    }

    num_intersection = 0;
    if (ray_patch_intersection_allow_backtracing(P, dir, prism_tri_1_c, prism_tri_1_a, prism_tri_2_a, prism_tri_2_c, &u, &v, &t, &u2, &v2, &t2, &num_intersection)) {
        if (handle_hit_patch_new(hit[total_intersection], Side::CA,
                         prism_tri_1_c, prism_tri_1_a, prism_tri_2_a, prism_tri_2_c,
                         tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
                         t, u, v, Edge::CA)) {
            total_intersection++;
        }

        if (num_intersection > 1
            && handle_hit_patch_new(hit[total_intersection], Side::CA,
                             prism_tri_1_c, prism_tri_1_a, prism_tri_2_a, prism_tri_2_c,
                             tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c,
                             t2, u2, v2, Edge::CA)) {
            total_intersection++;
        }
    }

    int ray_shift = 0;

    for (int i = 0; i < total_intersection - 1; i++) {
        for (int j = i + 1; j < total_intersection; j++) {
            if (hit[i].t > hit[j].t) {
                HitData_3 tmp = hit[i];
                hit[i] = hit[j];
                hit[j] = tmp;
            }
        }

        /*HitData_3 &min_hit = hit[i];
        HitData_3 &max_hit = hit[i + 1];

        if (! (((min_hit.t > 0 && ray_t > min_hit.t && min_hit.t < max_hit.t)
                 || (min_hit.t < 0.0f && max_hit.t > 0.0f)))) {
            ray_shift++;
        }*/
    }

//    for (int i = 0; i < total_intersection; i++) {
//        if (isect->t > hit[i].t && hit[i].t > 0.0f) {
//            isect->t = hit[i].t;
//            isect->object = object;
//            isect->prim = prim_addr;
//            isect->v = hit[i].u;
//            isect->v = hit[i].v;
//            isect->type = PRIMITIVE_TRIANGLE;
//        }
//    }

    if (ray_shift + 1 < total_intersection) {
        ++stack_ptr;
        assert(stack_ptr < BVH_STACK_SIZE);
        traversal_stack[stack_ptr] = node_addr;

        ++stack_ptr;
        assert(stack_ptr < BVH_STACK_SIZE);
        traversal_stack[stack_ptr] = object;

        node_addr = GEOPATTERN_SENTINEL;

        int node_addr = kernel_tex_fetch(__object_node, geopattern_link);
        geoframe.object_nodes = node_addr;
        geoframe.object = geopattern_link;
        assert(node_addr >= 0);
        float4 xx = kernel_tex_fetch(__bvh_nodes, node_addr + 1);
        float4 yy = kernel_tex_fetch(__bvh_nodes, node_addr + 2);
        float4 zz = kernel_tex_fetch(__bvh_nodes, node_addr + 3);

        // Отступы нужны
        float3 object_bbox_min = {
                min4(xx.x, xx.y, xx.z, xx.w),// + shift_eps,
                min4(yy.x, yy.y, yy.z, yy.w),// + shift_eps,
                min4(zz.x, zz.y, zz.z, zz.w)// - shift_eps
        };

        float3 object_bbox_max = {
                max4(xx.x, xx.y, xx.z, xx.w),// - shift_eps,
                max4(yy.x, yy.y, yy.z, yy.w),// - shift_eps,
                max4(zz.x, zz.y, zz.z, zz.w)
        };

        if (object_bbox_max.z == object_bbox_min.z) {
            object_bbox_max.z += geopatten_height;
        } else {
            object_bbox_min.z -= shift_eps;
            object_bbox_max.z += shift_eps;
        }

        float3 clipbox_bmin = float4_to_float3(kernel_tex_fetch(__geopattern_clipbox, geopattern_clipbox_link));
        float3 clipbox_bmax = float4_to_float3(kernel_tex_fetch(__geopattern_clipbox, geopattern_clipbox_link + 1));

        float x_size = object_bbox_max.x - object_bbox_min.x;
        float y_size = object_bbox_max.y - object_bbox_min.y;
        float z_size = object_bbox_max.z - object_bbox_min.z;

        float3 bbox_min;
        float3 bbox_max;
        if (geopattern_flags & GEOPATTERN_FLAGS_RANDOM_OFFSET) {
            float offset_x = ((float)hash(__float_as_int(tri_a.x), __float_as_int(tri_b.x), __float_as_int(tri_c.x)) / UINT32_MAX);
            float offset_y = ((float)hash(__float_as_int(tri_a.y), __float_as_int(tri_b.y), __float_as_int(tri_c.y)) / UINT32_MAX);

            float x_size = (clipbox_bmax.x - clipbox_bmin.x);
            float y_size = (clipbox_bmax.y - clipbox_bmin.y);

            clipbox_bmin.x += offset_x * 0.25 * x_size;
            clipbox_bmin.y += offset_y * 0.25 * y_size;
            clipbox_bmax.x -= offset_y * 0.25 * x_size;
            clipbox_bmax.y -= offset_x * 0.25 * y_size;

            if (clipbox_bmin.x > clipbox_bmax.x) {
                swap(clipbox_bmax.x, clipbox_bmin.x);
            }
            if (clipbox_bmin.y > clipbox_bmax.y) {
                swap(clipbox_bmax.y, clipbox_bmin.y);
            }
        }

        bbox_min.x = (clipbox_bmin.x * x_size) + object_bbox_min.x;
        bbox_min.y = (clipbox_bmin.y * y_size) + object_bbox_min.y;
        bbox_min.z = (clipbox_bmin.z * z_size) + object_bbox_min.z;

        bbox_max.x = (clipbox_bmax.x * x_size) + object_bbox_min.x;
        bbox_max.y = (clipbox_bmax.y * y_size) + object_bbox_min.y;
        bbox_max.z = (clipbox_bmax.z * z_size) + object_bbox_min.z;

        const float2 tri_a_uv = kernel_tex_fetch(__prim_tri_uv_geopattern, tri_vindex.w + 0);
        const float2 tri_b_uv = kernel_tex_fetch(__prim_tri_uv_geopattern, tri_vindex.w + 1);
        const float2 tri_c_uv = kernel_tex_fetch(__prim_tri_uv_geopattern, tri_vindex.w + 2);

        float2 A_uv = make_float2(device_lerp(bbox_min.x, bbox_max.x, tri_a_uv.x),
                                  device_lerp(bbox_min.y, bbox_max.y, tri_a_uv.y));

        float2 B_uv = make_float2(device_lerp(bbox_min.x, bbox_max.x, tri_b_uv.x),
                                  device_lerp(bbox_min.y, bbox_max.y, tri_b_uv.y));

        float2 C_uv = make_float2(device_lerp(bbox_min.x, bbox_max.x, tri_c_uv.x),
                                  device_lerp(bbox_min.y, bbox_max.y, tri_c_uv.y));
        geoframe.A = A_uv;
        geoframe.B = B_uv;
        geoframe.C = C_uv;

        if (object_flag & D_SD_OBJECT_NEGATIVE_SCALE_APPLIED) {
            geoframe.Q[0][0] = (tri_c - tri_a).x;
            geoframe.Q[1][0] = (tri_c - tri_a).y;
            geoframe.Q[2][0] = (tri_c - tri_a).z;

            geoframe.Q[0][1] = (tri_b - tri_a).x;
            geoframe.Q[1][1] = (tri_b - tri_a).y;
            geoframe.Q[2][1] = (tri_b - tri_a).z;

        } else {
            geoframe.Q[0][0] = (tri_b - tri_a).x;
            geoframe.Q[1][0] = (tri_b - tri_a).y;
            geoframe.Q[2][0] = (tri_b - tri_a).z;

            geoframe.Q[0][1] = (tri_c - tri_a).x;
            geoframe.Q[1][1] = (tri_c - tri_a).y;
            geoframe.Q[2][1] = (tri_c - tri_a).z;
        }

        float3 Ng = geopatten_height * normalize((tri_n_a + tri_n_b + tri_n_c) / 3.0f);
        geoframe.Q[0][2] = Ng.x;
        geoframe.Q[1][2] = Ng.y;
        geoframe.Q[2][2] = Ng.z;

        float2 e1 = (B_uv - A_uv);
        float2 e2 = (C_uv - A_uv);

        geoframe.QQ[0][0] = e1.x;
        geoframe.QQ[1][0] = e1.y;
        geoframe.QQ[2][0] = 0;

        geoframe.QQ[0][1] = e2.x;
        geoframe.QQ[1][1] = e2.y;
        geoframe.QQ[2][1] = 0;

        geoframe.QQ[0][2] = 0;
        geoframe.QQ[1][2] = 0;
        geoframe.QQ[2][2] = ((bbox_max.z - bbox_min.z) > 0.0)? (bbox_max.z - bbox_min.z) : 1.0f;

        geoframe.dir = dir;
        geoframe.P = P;
        geoframe.rays.num_rays = 0;
        geoframe.t_near = t_near;
        geoframe.t_far = t_far;
        geoframe.rays.current_ray = -1;
        geoframe.height = geopatten_height;
        geoframe.bbox_min_z = bbox_min.z;
        geoframe.bbox_max_z = bbox_max.z;

        int current_ray = 0;

        float3 _A_uv = make_float3(device_lerp(bbox_min.x, bbox_max.x, tri_a_uv.x),
                                   device_lerp(bbox_min.y, bbox_max.y, tri_a_uv.y),
                                   bbox_min.z);

        float3 _B_uv = make_float3(device_lerp(bbox_min.x, bbox_max.x, tri_b_uv.x),
                                   device_lerp(bbox_min.y, bbox_max.y, tri_b_uv.y),
                                   bbox_min.z);

        float3 _C_uv = make_float3(device_lerp(bbox_min.x, bbox_max.x, tri_c_uv.x),
                                   device_lerp(bbox_min.y, bbox_max.y, tri_c_uv.y),
                                   bbox_min.z);

        extend_borders(_A_uv, _B_uv, _C_uv);
        extend_borders(_B_uv, _A_uv, _C_uv);
        extend_borders(_C_uv, _A_uv, _B_uv);

        float3 _A_uv2 = make_float3(_A_uv.x,
                                    _A_uv.y,
                                   bbox_max.z);

        float3 _B_uv2 = make_float3(_B_uv.x,
                                    _B_uv.y,
                                    bbox_max.z);

        float3 _C_uv2 = make_float3(_C_uv.x,
                                    _C_uv.y,
                                    bbox_max.z);

        extend_borders_float2(geoframe.A, geoframe.B, geoframe.C);
        extend_borders_float2(geoframe.B, geoframe.A, geoframe.C);
        extend_borders_float2(geoframe.C, geoframe.B, geoframe.C);

        for (int i = ray_shift; i < total_intersection - 1; ++i) {
            HitData_3 &min_hit = hit[i];
            HitData_3 &max_hit = hit[i + 1];

            if (/*min_hit.side == max_hit.side||*/
               fabs(min_hit.t - max_hit.t) < EPS ||
               min_hit.t > isect->t ||
               max_hit.t < 0.0f) {
                continue;
            }

            if (min_hit.t < 0.0f) {
//                float t = -min_hit.t / (max_hit.t - min_hit.t);
//                float w = device_lerp(min_hit.w, max_hit.w, t);
//
//                const float3 T_a = device_lerp(tri_a, tri_a + tri_n_a, w);
//                const float3 T_b = device_lerp(tri_b, tri_b + tri_n_b, w);
//                const float3 T_c = device_lerp(tri_c, tri_c + tri_n_c, w);
//
//                barycentric(T_a, T_b, T_c, P, u, v);
//
//                if (!((u + v) <= 1.0f && (u + v) >= 0.0f)) {
//                    continue;
//                }
//
//                float d = sdistance_to_plane(T_a, T_b, T_c, 0, P);
//                if (std::abs(d) > 1e-4) {
//                    printf("!!! %f\n", d);
//                }
//
//                min_hit.p = P;
//                min_hit.t = 0.0f;
//                min_hit.u = u;
//                min_hit.v = v;
//                min_hit.w = w;

                float3 uvw = prism_point_uvw_coords(tri_a, tri_b, tri_c, tri_n_a, tri_n_b, tri_n_c, object_flag, P, EPS);
                min_hit.p = P;
                min_hit.t = 0.0f;
                min_hit.u = uvw.x;
                min_hit.v = uvw.y;
                min_hit.w = uvw.z;
                if (!((uvw.x + uvw.y) <= 1.0f && uvw.x >= 0.0f  && uvw.y >= 0.0f)) {
                    continue;
                }

                min_hit.side = Side::UVW;

                const float3 T_a = device_lerp(tri_a, tri_a + tri_n_a, min_hit.w);
                const float3 T_b = device_lerp(tri_b, tri_b + tri_n_b, min_hit.w);
                const float3 T_c = device_lerp(tri_c, tri_c + tri_n_c, min_hit.w);
                float d = sdistance_to_plane(T_a, T_b, T_c, 0, P);
                if (std::abs(d) > 1e-4f) {
                    printf("sdistance_to_plane %f\n", d);
                }
            }

#ifdef USE_TRIANGLE_MAPPING
            float2 uv_s = (1.0f - min_hit.u - min_hit.v) * tri_c_uv + min_hit.u * tri_a_uv + min_hit.v * tri_b_uv;
            float2 uv_e = (1.0f - max_hit.u - max_hit.v) * tri_c_uv + max_hit.u * tri_a_uv + max_hit.v * tri_b_uv;

            float3 start_texture = make_float3(device_lerp(bbox_min.x, bbox_max.x, uv_s.x),
                                               device_lerp(bbox_min.y, bbox_max.y, uv_s.y),
                                               device_lerp(bbox_min.z, bbox_max.z, min_hit.w));


            float3 end_texture = make_float3(device_lerp(bbox_min.x, bbox_max.x, uv_e.x),
                                             device_lerp(bbox_min.y, bbox_max.y, uv_e.y),
                                             device_lerp(bbox_min.z, bbox_max.z, max_hit.w));
#else
            float3 start_texture;
            float3 end_texture;
            switch (min_hit.side) {
                case Side::Top: {
                    start_texture = (1.0f - min_hit.u - min_hit.v) * _C_uv2 + min_hit.u * _A_uv2 + min_hit.v * _B_uv2;
                    break;
                }
                case Side::Down: {
                    start_texture = (1.0f - min_hit.u - min_hit.v) * _C_uv + min_hit.u * _A_uv + min_hit.v * _B_uv;
                    break;
                }
                case Side::AB: {
                    start_texture = bilinear_interpolation(_A_uv, _B_uv, _B_uv2, _A_uv2, min_hit.u, min_hit.v);
                    break;
                }
                case Side::BC: {
                    start_texture = bilinear_interpolation(_B_uv, _C_uv, _C_uv2, _B_uv2, min_hit.u, min_hit.v);
                    break;
                }
                case Side::CA: {
                    start_texture = bilinear_interpolation(_C_uv, _A_uv, _A_uv2, _C_uv2, min_hit.u, min_hit.v);
                    break;
                }
                case Side::UVW: {
                    float2 uv_s = (1.0f - min_hit.u - min_hit.v) * tri_c_uv + min_hit.u * tri_a_uv + min_hit.v * tri_b_uv;
                    start_texture = make_float3(device_lerp(bbox_min.x, bbox_max.x, uv_s.x),
                                                device_lerp(bbox_min.y, bbox_max.y, uv_s.y),
                                                device_lerp(bbox_min.z, bbox_max.z, min_hit.w));
                }
            }

            switch (max_hit.side) {
                case Side::Top: {
                    end_texture = (1.0f - max_hit.u - max_hit.v) * _C_uv2 + max_hit.u * _A_uv2 + max_hit.v * _B_uv2;
                    break;
                }
                case Side::Down: {
                    end_texture = (1.0f - max_hit.u - max_hit.v) * _C_uv + max_hit.u * _A_uv + max_hit.v * _B_uv;
                    break;
                }
                case Side::AB: {
                    end_texture = bilinear_interpolation(_A_uv, _B_uv, _B_uv2, _A_uv2, max_hit.u, max_hit.v);
                    break;
                }
                case Side::BC: {
                    end_texture = bilinear_interpolation(_B_uv, _C_uv, _C_uv2, _B_uv2, max_hit.u, max_hit.v);
                    break;
                }
                case Side::CA: {
                    end_texture = bilinear_interpolation(_C_uv, _A_uv, _A_uv2, _C_uv2, max_hit.u, max_hit.v);
                    break;
                }
            }
#endif
//нормали для Plane.000 все ещё выгнуты!

            float len_texture;
            float3 new_dir = normalize_len(end_texture - start_texture, &len_texture);

            float len_world = (max_hit.t - min_hit.t);

            float mult = len_texture / len_world;

//            if (min_hit.t < 0) {
//                start_texture += ((-min_hit.t + 100*EPS2)/(max_hit.t - min_hit.t)) * len_texture * dir;
//                len_texture = len(end_texture - start_texture);
//                len_world = max_hit.t;
//                mult = len_texture / len_world;
//                min_hit.t = 0.0f;
//            }

            start_texture -= offset_t * new_dir;


            if (!(mult > 0.0f)) {
                mult = 1.0f;
                printf("%f %f %f\n", mult, len_world, len_texture);
            }
            //assert(mult > 0.0f);
            geoframe.rays.t_min[current_ray] = min_hit.t;
            geoframe.rays.t_mult[current_ray] = mult;
            geoframe.rays.t_far[current_ray] = len_texture + offset_t + (EPS * len_texture);
            float min_t_uv = (-min_hit.t) * mult;
            geoframe.rays.t_near[current_ray] = (min_hit.t >= 0.0f) ? offset_t : min_t_uv + offset_t;
            geoframe.rays.P[current_ray] = start_texture;
            geoframe.rays.dir[current_ray] = new_dir;
            ++geoframe.rays.num_rays;
            ++current_ray;
        }
        return true;
    }
    return false;
}