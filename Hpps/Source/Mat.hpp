//
//  Mat.hpp
//  Garden
//
//  Created by Nycshisan on 2017/6/30.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef Mat_h
#define Mat_h

#include <cassert>
#include <x86intrin.h>

#include "Misc.hpp"

/*
 * Here are matrix definitions.
 * Always use 512-bit memory to store the martix regardless of the size of the matrix
 * to ensure accordance in the martix arithmetics.
 */

#define DEF_MAT(SIZE) \
typedef union { \
    __m128 col[4]; \
    struct { \
        data_t _11, _12, _13, _14, \
               _21, _22, _23, _24, \
               _31, _32, _33, _34, \
               _41, _42, _43, _44; \
    }; \
    ALWAYS_INLINE __m128& operator [] (size_t i) { \
        assert(i < SIZE); \
        return this->col[i]; \
    } \
    ALWAYS_INLINE __m128 operator [] (size_t i) const { \
        assert(i < SIZE); \
        return this->col[i]; \
    } \
} mat##SIZE __attribute__((aligned(16)))

DEF_MAT(2);
DEF_MAT(3);
DEF_MAT(4);

#undef DEF_MAT

// Matrix arithmetics
#define DEF_MAT_PLUSMINUS(SIZE, OPER, FUNC_NAME) \
ALWAYS_INLINE mat##SIZE operator OPER (const mat##SIZE &a, const mat##SIZE &b) { \
    mat##SIZE r; \
    for (size_t i = 0; i < SIZE; ++i) { \
        r.col[i] = _mm_##FUNC_NAME##_ps(a.col[i], b.col[i]);\
    } \
    return r; \
} \
ALWAYS_INLINE void operator OPER##= (mat##SIZE &a, const mat##SIZE &b) { \
    for (size_t i = 0; i < SIZE; ++i) { \
        a.col[i] = _mm_##FUNC_NAME##_ps(a.col[i], b.col[i]);\
    } \
}

DEF_MAT_PLUSMINUS(2, +, add)
DEF_MAT_PLUSMINUS(3, +, add)
DEF_MAT_PLUSMINUS(4, +, add)

DEF_MAT_PLUSMINUS(2, -, sub)
DEF_MAT_PLUSMINUS(3, -, sub)
DEF_MAT_PLUSMINUS(4, -, sub)

#undef DEF_MAT_PLUSMINUS

#define DEF_MAT_MULDIV(SIZE, OPER, FUNC_NAME) \
ALWAYS_INLINE mat##SIZE operator OPER (const mat##SIZE &v, data_t scalar) { \
    mat##SIZE r; \
    __m128 _s = _mm_set1_ps(scalar); \
    for (size_t i = 0; i < SIZE; ++i) { \
        r.col[i] = _mm_##FUNC_NAME##_ps(v.col[i], _s);\
    } \
    return r; \
} \
ALWAYS_INLINE void operator OPER##= (mat##SIZE &v, data_t scalar) { \
    __m128 _s = _mm_set1_ps(scalar); \
    for (size_t i = 0; i < SIZE; ++i) { \
        v.col[i] = _mm_##FUNC_NAME##_ps(v.col[i], _s);\
    } \
}

DEF_MAT_MULDIV(2, *, mul)
DEF_MAT_MULDIV(3, *, mul)
DEF_MAT_MULDIV(4, *, mul)

DEF_MAT_MULDIV(2, /, div)
DEF_MAT_MULDIV(3, /, div)
DEF_MAT_MULDIV(4, /, div)

#undef DEF_MAT_MULDIV

// Matrix dot product
#define DEF_MAT_DOT(SIZE) \
ALWAYS_INLINE vec##SIZE operator * (const mat##SIZE &m, const vec##SIZE &v) { \
    vec##SIZE r; \
    r.m = _mm_mul_ps(m.col[0], _mm_set1_ps(v.m[0])); \
    for (size_t i = 1; i < SIZE; ++i) { \
        r.m = _mm_add_ps(r.m, _mm_mul_ps(m.col[i], _mm_set1_ps(v.m[i]))); \
    } \
    return r; \
} \
ALWAYS_INLINE mat##SIZE operator * (mat##SIZE &a, const mat##SIZE &b) { \
    mat##SIZE r; \
    __m128 t, v; \
    for (size_t i = 0; i < SIZE; ++i) { \
        v = b.col[i]; \
        t = _mm_mul_ps(a.col[0], _mm_set1_ps(v[0])); \
        for (size_t j = 1; j < SIZE; ++j) { \
            t = _mm_add_ps(t, _mm_mul_ps(a.col[j], _mm_set1_ps(v[j]))); \
        } \
        r.col[i] = t; \
    } \
    return r; \
}

DEF_MAT_DOT(2)
DEF_MAT_DOT(3)
DEF_MAT_DOT(4)

#undef DEF_MAT_DOT

// Matrix transpose
// We could use the same function to transpose mat2/3/4 because we store them in an accordance 512-bit container
// t0 = {_11, _21, _12, _22}
// t1 = {_13, _23, _14, _24}
// t2 = {_31, _41, _32, _42}
// t3 = {_33, _43, _34, _44}
// col[0] = {_11, _21, _31, _41}
// col[1] = {_12, _22, _32, _42}
// col[2] = {_13, _23, _33, _43}
// col[3] = {_13, _24, _34, _44}
#define DEF_MAT_TRANSPOSE(SIZE) \
ALWAYS_INLINE void transpose(mat##SIZE &m) { \
    __m128 t0 = _mm_unpacklo_ps(m.col[0], m.col[1]); \
    __m128 t1 = _mm_unpackhi_ps(m.col[0], m.col[1]); \
    __m128 t2 = _mm_unpacklo_ps(m.col[2], m.col[3]); \
    __m128 t3 = _mm_unpackhi_ps(m.col[2], m.col[3]); \
    m.col[0] = _mm_movelh_ps(t0, t2); \
    m.col[1] = _mm_movehl_ps(t2, t0); \
    m.col[2] = _mm_movelh_ps(t1, t3); \
    m.col[3] = _mm_movehl_ps(t3, t1); \
}

DEF_MAT_TRANSPOSE(2)
DEF_MAT_TRANSPOSE(3)
DEF_MAT_TRANSPOSE(4)

#undef DEF_MAT_TRANSPOSE

// Matrix inverse
ALWAYS_INLINE void invert(mat2 &m) {
    // Calculate the determinant
    __m128 det, tmp;
    tmp = _mm_shuffle_ps(m.col[1], _mm_undefined_ps(), 0x01);
    det = _mm_mul_ps(m.col[0], tmp);
    det = _mm_hsub_ps(det, det);
    
    tmp = _mm_rcp_ss(det);
    
    det = _mm_sub_ss(_mm_add_ss(tmp, tmp), _mm_mul_ss(det, _mm_mul_ss(tmp, tmp)));
    det = _mm_shuffle_ps(det, det, 0x00);
    
    // Convert to adjoint matrix
    data_t t = m._11;
    m._11 = m._22;
    m._22 = t;
    
    m._12 = -m._12;
    m._21 = -m._21;
    
    // Apply the determinant
    m.col[0] = _mm_mul_ps(m.col[0], det);
    m.col[1] = _mm_mul_ps(m.col[1], det);
}

ALWAYS_INLINE void invert(mat3 &m) {
    // Have not got a easy way for 3x3 matrix inversion, Use the algorithm for 4x4 matrixes.
    
    // Fill the fourth order
    static __m128 _row3 = {0, 0, 0, 1};
    m.col[3] = _row3;
    m._41 = m._42 = m._43 = 0;
    
    __m128 minor0, minor1, minor2, tmp, det;
    __m128 row0, row1, row2, row3;
    
    tmp = _mm_movelh_ps(m.col[0], m.col[1]);
    row1 = _mm_movelh_ps(m.col[2], m.col[3]);
    
    row0 = _mm_shuffle_ps(tmp, row1, 0x88);
    row1 = _mm_shuffle_ps(row1, tmp, 0xDD);
    
    tmp = _mm_movehl_ps(m.col[1], m.col[0]);
    row3 = _mm_movehl_ps(m.col[3], m.col[2]);
    
    row2 = _mm_shuffle_ps(tmp, row3, 0x88);
    row3 = _mm_shuffle_ps(row3, tmp, 0xDD);
    
    // Calculate the minors
    tmp = _mm_mul_ps(row2, row3);
    tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);
    
    minor0 = _mm_mul_ps(row1, tmp);
    minor1 = _mm_mul_ps(row0, tmp);
    
    tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);
    
    minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp), minor0);
    minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp), minor1);
    minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);
    // ---
    tmp = _mm_mul_ps(row1, row2);
    tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);
    
    minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp), minor0);
    
    tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);
    
    minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp));
    // ---
    tmp = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
    tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);
    row2 = _mm_shuffle_ps(row2, row2, 0x4E);
    
    minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp), minor0);
    minor2 = _mm_mul_ps(row0, tmp);
    
    tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);
    
    minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp));
    minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp), minor2);
    minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);
    // ---
    tmp = _mm_mul_ps(row0, row1);
    tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);
    
    minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp), minor2);
    
    tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);
    
    minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp), minor2);
    // ---
    tmp = _mm_mul_ps(row0, row3);
    tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);
    
    minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp));
    minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp), minor2);
    
    tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);
    
    minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp), minor1);
    minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp));
    // ---
    tmp = _mm_mul_ps(row0, row2);
    tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);
    
    minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp), minor1);
    
    tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);
    
    minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp));
    
    
    // Calculate the determinant
    det = _mm_mul_ps(row0, minor0);
    det[3] = 0.0;
    det = _mm_hadd_ps(det, det);
    det = _mm_hadd_ps(det, det);
    
    tmp = _mm_rcp_ss(det);
    
    det = _mm_sub_ss(_mm_add_ss(tmp, tmp), _mm_mul_ss(det, _mm_mul_ss(tmp, tmp)));
    det = _mm_shuffle_ps(det, det, 0x00);
    
    // Apply the determinant
    m.col[0] = _mm_mul_ps(minor0, det);
    m.col[1] = _mm_mul_ps(minor1, det);
    m.col[2] = _mm_mul_ps(minor2, det);
}

// Referenced: http://swiborg.com/download/dev/pdf/simd-inverse-of-4x4-matrix.pdf
ALWAYS_INLINE void invert(mat4 &m) {
    __m128 minor0, minor1, minor2, minor3, tmp, det;
    __m128 row0, row1, row2, row3;
    
    tmp = _mm_movelh_ps(m.col[0], m.col[1]);
    row1 = _mm_movelh_ps(m.col[2], m.col[3]);
    
    row0 = _mm_shuffle_ps(tmp, row1, 0x88);
    row1 = _mm_shuffle_ps(row1, tmp, 0xDD);
    
    tmp = _mm_movehl_ps(m.col[1], m.col[0]);
    row3 = _mm_movehl_ps(m.col[3], m.col[2]);
    
    row2 = _mm_shuffle_ps(tmp, row3, 0x88);
    row3 = _mm_shuffle_ps(row3, tmp, 0xDD);
    
    // Calculate the minors
    tmp = _mm_mul_ps(row2, row3);
    tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);
    
    minor0 = _mm_mul_ps(row1, tmp);
    minor1 = _mm_mul_ps(row0, tmp);
    
    tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);
    
    minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp), minor0);
    minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp), minor1);
    minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);
    // ---
    tmp = _mm_mul_ps(row1, row2);
    tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);
    
    minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp), minor0);
    minor3 = _mm_mul_ps(row0, tmp);
    
    tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);
    
    minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp));
    minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp), minor3);
    minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);
    // ---
    tmp = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
    tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);
    row2 = _mm_shuffle_ps(row2, row2, 0x4E);
    
    minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp), minor0);
    minor2 = _mm_mul_ps(row0, tmp);
    
    tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);
    
    minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp));
    minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp), minor2);
    minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);

    tmp = _mm_mul_ps(row0, row1);
    tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);
    
    minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp), minor2);
    minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp), minor3);
    
    tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);
    
    minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp), minor2);
    minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp));
    // ---
    tmp = _mm_mul_ps(row0, row3);
    tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);
    
    minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp));
    minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp), minor2);
    
    tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);
    
    minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp), minor1);
    minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp));
    // ---
    tmp = _mm_mul_ps(row0, row2);
    tmp = _mm_shuffle_ps(tmp, tmp, 0xB1);
    
    minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp), minor1);
    minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp));
    
    tmp = _mm_shuffle_ps(tmp, tmp, 0x4E);
    
    minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp));
    minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp), minor3);
    
    
    // Calculate the determinant
    det = _mm_mul_ps(row0, minor0);
    det = _mm_hadd_ps(det, det);
    det = _mm_hadd_ps(det, det);
    
    tmp = _mm_rcp_ss(det);
    
    det = _mm_sub_ss(_mm_add_ss(tmp, tmp), _mm_mul_ss(det, _mm_mul_ss(tmp, tmp)));
    det = _mm_shuffle_ps(det, det, 0x00);
    
    // Apply the determinant
    m.col[0] = _mm_mul_ps(minor0, det);
    m.col[1] = _mm_mul_ps(minor1, det);
    m.col[2] = _mm_mul_ps(minor2, det);
    m.col[3] = _mm_mul_ps(minor3, det);
}

#endif /* Mat_h */
