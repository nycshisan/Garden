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

#define VEC4_NAME x, y, z, w
#define VEC3_NAME x, y, z
#define VEC2_NAME x, y

#define DEF_VEC(SIZE) \
typedef union { \
    data_t d[SIZE]; \
    struct { \
        data_t VEC##SIZE##_NAME; \
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


#endif /* Vec_h */
