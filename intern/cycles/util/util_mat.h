#pragma once
#ifdef __CUDA_ARCH__
#define FUNC_PREF __device__ __host__
#else
#define  FUNC_PREF
#endif

struct Mat {
	float m[4][4];

    FUNC_PREF float *operator[](int idx) const {
		return (float *)m[idx];
	}
};

struct Mat3 {
    float m[3][3];

    FUNC_PREF float *operator[](int idx) const {
        return (float *)m[idx];
    }
};

ccl_device_inline float det(Mat3 const &m) {
    float det = m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) -
                m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
                m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
    return det;
}

ccl_device_inline Mat3 inverse(Mat3 const &m) {
    float det = m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) -
                m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
                m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);

    float invdet = 1.0f / det;

    Mat3 minv; // inverse of matrix m
    minv[0][0] = (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * invdet;
    minv[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) * invdet;
    minv[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * invdet;
    minv[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * invdet;
    minv[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * invdet;
    minv[1][2] = (m[1][0] * m[0][2] - m[0][0] * m[1][2]) * invdet;
    minv[2][0] = (m[1][0] * m[2][1] - m[2][0] * m[1][1]) * invdet;
    minv[2][1] = (m[2][0] * m[0][1] - m[0][0] * m[2][1]) * invdet;
    minv[2][2] = (m[0][0] * m[1][1] - m[1][0] * m[0][1]) * invdet;
    return minv;
}

ccl_device_inline Mat inverse(Mat const &m)  {
    float res[4][4];
    float tmp[12]; /* temp array for pairs */
    float src[16]; /* array of transpose source matrix */
    float det; /* determinant */
    /* transpose matrix */
    for (int i = 0; i < 4; i++) {
        src[i + 0] = m[i][0];
        src[i + 4] = m[i][1];
        src[i + 8] = m[i][2];
        src[i + 12] = m[i][3];
    }
    tmp[0] = src[10] * src[15];
    tmp[1] = src[11] * src[14];
    tmp[2] = src[9] * src[15];
    tmp[3] = src[11] * src[13];
    tmp[4] = src[9] * src[14];
    tmp[5] = src[10] * src[13];
    tmp[6] = src[8] * src[15];
    tmp[7] = src[11] * src[12];
    tmp[8] = src[8] * src[14];
    tmp[9] = src[10] * src[12];
    tmp[10] = src[8] * src[13];
    tmp[11] = src[9] * src[12];
    res[0][0] = tmp[0] * src[5] + tmp[3] * src[6] + tmp[4] * src[7];
    res[0][0] -= tmp[1] * src[5] + tmp[2] * src[6] + tmp[5] * src[7];
    res[0][1] = tmp[1] * src[4] + tmp[6] * src[6] + tmp[9] * src[7];
    res[0][1] -= tmp[0] * src[4] + tmp[7] * src[6] + tmp[8] * src[7];
    res[0][2] = tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7];
    res[0][2] -= tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7];
    res[0][3] = tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6];
    res[0][3] -= tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6];
    res[1][0] = tmp[1] * src[1] + tmp[2] * src[2] + tmp[5] * src[3];
    res[1][0] -= tmp[0] * src[1] + tmp[3] * src[2] + tmp[4] * src[3];
    res[1][1] = tmp[0] * src[0] + tmp[7] * src[2] + tmp[8] * src[3];
    res[1][1] -= tmp[1] * src[0] + tmp[6] * src[2] + tmp[9] * src[3];
    res[1][2] = tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3];
    res[1][2] -= tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3];
    res[1][3] = tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2];
    res[1][3] -= tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2];
    tmp[0] = src[2] * src[7];
    tmp[1] = src[3] * src[6];
    tmp[2] = src[1] * src[7];
    tmp[3] = src[3] * src[5];
    tmp[4] = src[1] * src[6];
    tmp[5] = src[2] * src[5];
    tmp[6] = src[0] * src[7];
    tmp[7] = src[3] * src[4];
    tmp[8] = src[0] * src[6];
    tmp[9] = src[2] * src[4];
    tmp[10] = src[0] * src[5];
    tmp[11] = src[1] * src[4];
    res[2][0] = tmp[0] * src[13] + tmp[3] * src[14] + tmp[4] * src[15];
    res[2][0] -= tmp[1] * src[13] + tmp[2] * src[14] + tmp[5] * src[15];
    res[2][1] = tmp[1] * src[12] + tmp[6] * src[14] + tmp[9] * src[15];
    res[2][1] -= tmp[0] * src[12] + tmp[7] * src[14] + tmp[8] * src[15];
    res[2][2] = tmp[2] * src[12] + tmp[7] * src[13] + tmp[10] * src[15];
    res[2][2] -= tmp[3] * src[12] + tmp[6] * src[13] + tmp[11] * src[15];
    res[2][3] = tmp[5] * src[12] + tmp[8] * src[13] + tmp[11] * src[14];
    res[2][3] -= tmp[4] * src[12] + tmp[9] * src[13] + tmp[10] * src[14];
    res[3][0] = tmp[2] * src[10] + tmp[5] * src[11] + tmp[1] * src[9];
    res[3][0] -= tmp[4] * src[11] + tmp[0] * src[9] + tmp[3] * src[10];
    res[3][1] = tmp[8] * src[11] + tmp[0] * src[8] + tmp[7] * src[10];
    res[3][1] -= tmp[6] * src[10] + tmp[9] * src[11] + tmp[1] * src[8];
    res[3][2] = tmp[6] * src[9] + tmp[11] * src[11] + tmp[3] * src[8];
    res[3][2] -= tmp[10] * src[11] + tmp[2] * src[8] + tmp[7] * src[9];
    res[3][3] = tmp[10] * src[10] + tmp[4] * src[8] + tmp[9] * src[9];
    res[3][3] -= tmp[8] * src[9] + tmp[11] * src[10] + tmp[5] * src[8];
    det = src[0] * res[0][0] + src[1] * res[0][1] + src[2] * res[0][2] + src[3] * res[0][3];
    det = 1.0f / det;
    Mat fres;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            fres[i][j] = res[i][j] * det;
        }
    }
    return fres;
}

ccl_device_inline Mat3 transpose(Mat3 const &m)
{
    Mat3 res;
    for(int i = 0; i < 3; i++)
    {
        for(int i2 = 0; i2 < 3; i2++)
        {
            res[i2][i] = m[i][i2];
        }
    }
    return res;
}

ccl_device_inline Mat transpose(Mat const &m)
{
    Mat res;
    for(int i = 0; i < 4; i++)
    {
        for(int i2 = 0; i2 < 4; i2++)
        {
            res[i2][i] = m[i][i2];
        }
    }
    return res;
}

ccl_device_inline float4 operator*(Mat const &m, float4 const v) {
	float4 result = make_float4(0, 0, 0, 0);
    result.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
    result.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
    result.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
    result.w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;
    return result;
}

ccl_device_inline float3 operator*(Mat3 const &m, float3 const v) {
    float3 result = make_float3(0, 0, 0);
    result.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z;
    result.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z;
    result.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z;
    return result;
}

ccl_device_inline Mat operator*(Mat const &m, Mat const &m2) {
    Mat result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                result[i][j] += (m[i][k] * m2[k][j]);
            }
        }
    }
    return result;
}

ccl_device_inline Mat3 operator*(Mat3 const &m, Mat3 const &m2) {
    Mat3 result;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result[i][j] = 0;
            for (int k = 0; k < 3; k++) {
                result[i][j] += (m[i][k] * m2[k][j]);
            }
        }
    }
    return result;
}

ccl_device void dump_matrix(Mat const &m) {
    printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n\n",
           (double)m[0][0], (double)m[0][1], (double)m[0][2], (double)m[0][3],
           (double)m[1][0], (double)m[1][1], (double)m[1][2], (double)m[1][3],
           (double)m[2][0], (double)m[2][1], (double)m[2][2], (double)m[2][3],
           (double)m[3][0], (double)m[3][1], (double)m[3][2], (double)m[3][3]);
}

ccl_device void dump_matrix(Mat3 const &m) {
    printf("%f %f %f\n%f %f %f\n%f %f %f\n\n",
           (double)m[0][0], (double)m[0][1], (double)m[0][2],
           (double)m[1][0], (double)m[1][1], (double)m[1][2],
           (double)m[2][0], (double)m[2][1], (double)m[2][2]);
}
