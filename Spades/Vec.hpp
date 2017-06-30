//
//  Vec.hpp
//  Garden
//
//  Created by Nycshisan on 2017/5/3.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef Vec_h
#define Vec_h

#include <cassert>
#include <x86intrin.h>

#include "Misc.hpp"

/* 
 * Here are vector definitions.
 * Always use 128-bit to store the vector regardless of the size of the vector
 * to ensure accordance in the vector arithmetics.
 */

#define VEC4_DIMS x, y, z, w
#define VEC3_DIMS x, y, z
#define VEC2_DIMS x, y

#define DEF_VEC(SIZE) \
typedef union { \
    __m128 m128; \
    data_t d[SIZE]; \
    struct { \
        data_t VEC##SIZE##_DIMS; \
    }; \
    inline data_t& operator [] (size_t i) { \
        assert(i < SIZE); \
        return this->d[i]; \
    } \
    inline data_t operator [] (size_t i) const { \
        assert(i < SIZE); \
        return this->d[i]; \
    } \
} vec##SIZE __attribute__((aligned(16)));

DEF_VEC(4);
DEF_VEC(3);
DEF_VEC(2);

#undef DEF_VEC

// Vector arithmetics
#define DEF_VEC_PLUSMINUS(SIZE, OPER, FUNC_NAME) \
__attribute__((always_inline)) vec##SIZE operator OPER (const vec##SIZE &a, const vec##SIZE &b) { \
    vec##SIZE r; \
    r.m128 = _mm_##FUNC_NAME##_ps(a.m128, b.m128); \
    return r; \
} \
__attribute__((always_inline)) void operator OPER##= (vec##SIZE &a, const vec##SIZE &b) { \
    a.m128 = _mm_##FUNC_NAME##_ps(a.m128, b.m128); \
}

DEF_VEC_PLUSMINUS(2, +, add)
DEF_VEC_PLUSMINUS(3, +, add)
DEF_VEC_PLUSMINUS(4, +, add)

DEF_VEC_PLUSMINUS(2, -, sub)
DEF_VEC_PLUSMINUS(3, -, sub)
DEF_VEC_PLUSMINUS(4, -, sub)

#undef DEF_VEC_PLUSMINUS

#define DEF_VEC_MULDIV(SIZE, OPER, FUNC_NAME) \
__attribute__((always_inline)) vec##SIZE operator OPER (const vec##SIZE &v, const data_t &scalar) { \
    vec##SIZE r; \
    __m128 _s = _mm_set1_ps(scalar); \
    r.m128 = _mm_##FUNC_NAME##_ps(v.m128, _s); \
    return r; \
} \
__attribute__((always_inline)) void operator OPER##= (vec##SIZE &v, const data_t &scalar) { \
    __m128 _s = _mm_set1_ps(scalar); \
    v.m128 = _mm_##FUNC_NAME##_ps(v.m128, _s); \
}

DEF_VEC_MULDIV(2, *, mul)
DEF_VEC_MULDIV(3, *, mul)
DEF_VEC_MULDIV(4, *, mul)

DEF_VEC_MULDIV(2, /, div)
DEF_VEC_MULDIV(3, /, div)
DEF_VEC_MULDIV(4, /, div)

#undef DEF_VEC_MULDIV


#endif /* Vec_h */
