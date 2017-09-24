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
 * Always use 128-bit memory to store the vector regardless of the size of the vector
 * to ensure accordance in the vector arithmetics.
 */

#define DEF_VEC(SIZE) \
typedef union { \
    __m128 m; \
    data_t data[SIZE]; \
    struct { \
        data_t x, y, z, w; \
    }; \
    struct { \
        data_t r, g, b, a; \
    }; \
    ALWAYS_INLINE data_t& operator [] (size_t i) { \
        assert(i < SIZE); \
        return this->data[i]; \
    } \
    ALWAYS_INLINE data_t operator [] (size_t i) const { \
        assert(i < SIZE); \
        return this->data[i]; \
    } \
} vec##SIZE __attribute__((aligned(16)))

DEF_VEC(4);
DEF_VEC(3);
DEF_VEC(2);

#undef DEF_VEC

// Vector arithmetics
#define DEF_VEC_PLUSMINUS(SIZE, OPER, FUNC_NAME) \
ALWAYS_INLINE vec##SIZE operator OPER (const vec##SIZE &a, const vec##SIZE &b) { \
    vec##SIZE r; \
    r.m = _mm_##FUNC_NAME##_ps(a.m, b.m); \
    return r; \
} \
ALWAYS_INLINE void operator OPER##= (vec##SIZE &a, const vec##SIZE &b) { \
    a.m = _mm_##FUNC_NAME##_ps(a.m, b.m); \
}

DEF_VEC_PLUSMINUS(2, +, add)
DEF_VEC_PLUSMINUS(3, +, add)
DEF_VEC_PLUSMINUS(4, +, add)

DEF_VEC_PLUSMINUS(2, -, sub)
DEF_VEC_PLUSMINUS(3, -, sub)
DEF_VEC_PLUSMINUS(4, -, sub)

#undef DEF_VEC_PLUSMINUS

#define DEF_VEC_MULDIV(SIZE, OPER, FUNC_NAME) \
ALWAYS_INLINE vec##SIZE operator OPER (const vec##SIZE &v, data_t scalar) { \
    vec##SIZE r; \
    __m128 _s = _mm_set1_ps(scalar); \
    r.m = _mm_##FUNC_NAME##_ps(v.m, _s); \
    return r; \
} \
ALWAYS_INLINE void operator OPER##= (vec##SIZE &v, data_t scalar) { \
    __m128 _s = _mm_set1_ps(scalar); \
    v.m = _mm_##FUNC_NAME##_ps(v.m, _s); \
}

DEF_VEC_MULDIV(2, *, mul)
DEF_VEC_MULDIV(3, *, mul)
DEF_VEC_MULDIV(4, *, mul)

DEF_VEC_MULDIV(2, /, div)
DEF_VEC_MULDIV(3, /, div)
DEF_VEC_MULDIV(4, /, div)

#undef DEF_VEC_MULDIV

// Vector dot product
#define DOT_CONTROL_2 0b00111111
#define DOT_CONTROL_3 0b01111111
#define DOT_CONTROL_4 0b11111111

#define DEF_VEC_DOT(SIZE) \
ALWAYS_INLINE __m128 __dot(const vec##SIZE &a, const vec##SIZE &b) { \
    return _mm_dp_ps(a.m, b.m, DOT_CONTROL_##SIZE); \
} \
ALWAYS_INLINE data_t dot(const vec##SIZE &a, const vec##SIZE &b) { \
    return __dot(a, b)[0]; \
}

DEF_VEC_DOT(2)
DEF_VEC_DOT(3)
DEF_VEC_DOT(4)

#undef DOT_CONTROL_2
#undef DOT_CONTROL_3
#undef DOT_CONTROL_4

#undef DEF_VEC_DOT


// Frequently used constant vectors
namespace Color {
    vec4 black = {0.0, 0.0, 0.0, 1.0};
    vec4 white = {1.0, 1.0, 1.0, 1.0};
    vec4 red = {1.0, 0.0, 0.0, 1.0};
    vec4 green = {0.0, 1.0, 0.0, 1.0};
    vec4 blue = {0.0, 0.0, 1.0, 1.0};
    vec4 cyan = {0.0, 1.0, 1.0, 1.0};
}

#endif /* Vec_h */
