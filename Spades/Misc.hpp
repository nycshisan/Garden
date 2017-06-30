//
//  Misc.hpp
//  Garden
//
//  Created by Nycshisan on 2017/6/29.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef Misc_h
#define Misc_h

typedef float data_t;

#define fatalError(MSG) do {\
    printf(MSG);\
    assert(0);\
} while(0)

/* 
 * Cuz C++ does not have reflections, the implementation of Varying is ugly and unfriendly.
 * Any Varying class should contain the VARYING_DEF_HELPER macro.
 * Pipeliens which do not using Varying could be specialized by the NullVarying class.
 */
#define VARYING_DEF_HELPER(CLASS_NAME, ADD_DEF, SUB_DEF, MUL_DEF) template <class Varying> friend class Rasterizer; \
VARYING_ADD_DEF(CLASS_NAME, ADD_DEF) \
VARYING_SUB_DEF(CLASS_NAME, SUB_DEF) \
VARYING_MUL_DEF(MUL_DEF)

#define VARYING_ADD_DEF(CLASS_NAME, ADD_DEF) void add(CLASS_NAME &addend) ADD_DEF
#define VARYING_SUB_DEF(CLASS_NAME, SUB_DEF) void subtract(CLASS_NAME &subtrahend) SUB_DEF
#define VARYING_MUL_DEF(MUL_DEF) void multiply(data_t factor) MUL_DEF

class NullVarying {
    VARYING_DEF_HELPER(NullVarying, {}, {}, {})
};

#endif /* Misc_h */
