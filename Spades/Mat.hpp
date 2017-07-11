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
ALWAYS_INLINE mat##SIZE operator OPER (mat##SIZE a, mat##SIZE b) { \
    mat##SIZE r; \
    for (size_t i = 0; i < SIZE; ++i) { \
        r.col[i] = _mm_##FUNC_NAME##_ps(a.col[i], b.col[i]);\
    } \
    return r; \
} \
ALWAYS_INLINE void operator OPER##= (mat##SIZE a, mat##SIZE b) { \
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
ALWAYS_INLINE mat##SIZE operator OPER (mat##SIZE v, data_t scalar) { \
    mat##SIZE r; \
    __m128 _s = _mm_set1_ps(scalar); \
    for (size_t i = 0; i < SIZE; ++i) { \
        r.col[i] = _mm_##FUNC_NAME##_ps(v.col[i], _s);\
    } \
    return r; \
} \
ALWAYS_INLINE void operator OPER##= (mat##SIZE v, data_t scalar) { \
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
ALWAYS_INLINE vec##SIZE operator * (mat##SIZE m, vec##SIZE v) { \
    vec##SIZE r; \
    r.m = _mm_mul_ps(m.col[0], v.m); \
    for (size_t i = 1; i < SIZE; ++i) { \
        r.m = _mm_add_ps(r.m, _mm_mul_ps(m.col[i], v.m)); \
    } \
    return r; \
} \
ALWAYS_INLINE mat##SIZE operator * (mat##SIZE a, mat##SIZE b) { \
    mat##SIZE r; \
    __m128 t, v; \
    for (size_t i = 0; i < SIZE; ++i) { \
        v = b.col[i]; \
        t = _mm_mul_ps(a.col[0], v); \
        for (size_t i = 1; i < SIZE; ++i) { \
            t = _mm_add_ps(t, _mm_mul_ps(a.col[i], v)); \
        } \
        r.col[i] = t; \
    } \
    return r; \
}

DEF_MAT_DOT(2)
DEF_MAT_DOT(3)
DEF_MAT_DOT(4)

#undef DEF_MAT_DOT

// Matrix inverse
ALWAYS_INLINE void invert(mat2 m) {
    
}

ALWAYS_INLINE void invert(mat3 m) {

}

ALWAYS_INLINE void invert(mat4 m) {

}

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
ALWAYS_INLINE void transpose(mat##SIZE m) { \
    __m128 t0 = _mm_unpacklo_ps(m.col[0], m.col[1]); \
    __m128 t1 = _mm_unpackhi_ps(m.col[0], m.col[1]); \
    __m128 t2 = _mm_unpacklo_ps(m.col[2], m.col[3]); \
    __m128 t3 = _mm_unpackhi_ps(m.col[2], m.col[3]); \
    m.col[0] = _mm_movelh_ps(t0, t2); \
    m.col[1] = _mm_movehl_ps(t0, t2); \
    m.col[2] = _mm_movelh_ps(t1, t3); \
    m.col[3] = _mm_movehl_ps(t1, t3); \
}

DEF_MAT_TRANSPOSE(2)
DEF_MAT_TRANSPOSE(3)
DEF_MAT_TRANSPOSE(4)

#undef DEF_MAT_TRANSPOSE

#endif /* Mat_h */
