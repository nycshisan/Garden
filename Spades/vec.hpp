//
//  vec.hpp
//  Garden
//
//  Created by Nycshisan on 2017/5/3.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef vec_h
#define vec_h

#include <cassert>
#include <cstdarg>

#define DEFINE_VEC_ARITH_METHOD(NAME, OPER) \
vec<size> & NAME(const vec<size> &v) {\
    for (int i = 0; i < size; ++i) {\
        (*this)[i] OPER v[i];\
    }\
    return *this;\
}

#define DEFINE_VEC_ARITH_OPERATOR(NAME, OPER) \
template <int size>\
vec<size> operator OPER (const vec<size> &vl, const vec<size> &vr) {\
    vec<size> v;\
    for (int i = 0; i < size; ++i) {\
        v[i] = vl[i] OPER vr[i];\
    }\
    return v;\
}\
template <int size>\
vec<size> operator OPER (vec<size> &&vl, const vec<size> &vr) {\
    return std::move(vl.NAME(vr));\
}\
template <int size>\
vec<size> operator OPER (const vec<size> &vl, vec<size> &&vr) {\
    return std::move(vr.NAME(vl));\
}\
template <int size>\
vec<size> operator OPER (vec<size> &&vl, vec<size> &&vr) {\
    return std::move(vl.NAME(vr));\
}

typedef float data_t;

template <int size>
class vec {
    data_t *d_ptr;
    int offset = 1;
    bool own_memory = false;
    
public:
    vec() {
        this->d_ptr = new data_t[size];
        this->own_memory = true;
    }
    
    vec(data_t *d_ptr, int offset): d_ptr(d_ptr), offset(offset) {
        this->own_memory = false;
    }
    
    vec(data_t d, ...) {
        va_list ap;
        va_start(ap, d);
        this->d_ptr = new data_t[size];
        this->own_memory = true;
        *d_ptr = d;
        for (int i = 1; i < size; ++i) {
            d_ptr[i] = data_t(va_arg(ap, double));
        }
        va_end(ap);
    }
    
    vec(const vec<size> &v) {
        this->d_ptr = new data_t[size];
        this->own_memory = true;
        for (int i = 0; i < size; ++i) {
            this->d_ptr[i] = v[i];
        }
    }
    
    vec<size>& operator = (const vec<size> &v) {
        if (this->own_memory) {
            delete [] this->d_ptr;
        }
        this->d_ptr = new data_t[size];
        this->own_memory = true;
        for (int i = 0; i < size; ++i) {
            this->d_ptr[i] = v[i];
        }
        return *this;
    }
    
    vec(vec<size> &&v) {
        if (this == &v) {
            return;
        }
        if (this->own_memory) {
            delete [] this->d_ptr;
        }
        this->d_ptr = v.d_ptr;
        this->own_memory = v.own_memory;
        v.own_memory = false;
    }
    
    ~vec() {
        if (own_memory) {
            delete [] d_ptr;
        }
    }
    
    DEFINE_VEC_ARITH_METHOD(add, +=)
    DEFINE_VEC_ARITH_METHOD(sub, -=)
    DEFINE_VEC_ARITH_METHOD(mul, *=)
    DEFINE_VEC_ARITH_METHOD(div, /=)
    
    data_t dot(const vec<size> & v) {
        data_t sum = 0;
        for (int i = 0; i < size; ++i) {
            sum += (*this)[i] * v[i];
        }
        return sum;
    }
    
    inline data_t& operator [](const int i) const {
        assert(i < size);
        return *(d_ptr + i * offset);
    }
};

DEFINE_VEC_ARITH_OPERATOR(add, +)
DEFINE_VEC_ARITH_OPERATOR(sub, -)
DEFINE_VEC_ARITH_OPERATOR(mul, *)
DEFINE_VEC_ARITH_OPERATOR(div, /)


#undef DEFINE_VEC_ARITH_METHOD
#undef DEFINE_VEC_ARITH_OPERATOR

#endif /* vec_h */
