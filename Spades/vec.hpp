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
#include <vector>
#include <utility>

#define DEFINE_VEC_ARITH_METHOD(NAME, OPER) \
vec<size> & NAME(const vec<size> & v) {\
    for (int i = 0; i < size; ++i) {\
        (*this)[i] OPER v[i];\
    }\
    return *this;\
}

#define DEFINE_VEC_ARITH_OPERATOR(NAME, OPER) \
template <int size>\
vec<size> operator OPER (const vec<size> & vl, const vec<size> & vr) {\
    vec<size> v;\
    for (int i = 0; i < size; ++i) {\
        v[i] = vl[i] OPER vr[i];\
    }\
    return v;\
}\
template <int size>\
vec<size> operator OPER (vec<size> && vl, const vec<size> & vr) {\
    return std::move(vl.NAME(vr));\
}\
template <int size>\
vec<size> operator OPER (const vec<size> & vl, vec<size> && vr) {\
    return std::move(vr.NAME(vl));\
}\
template <int size>\
vec<size> operator OPER (vec<size> && vl, vec<size> && vr) {\
    return std::move(vl.NAME(vr));\
}

typedef double data_t;

template <int size>
class vec {
    data_t *d_ptr;
    bool own_memory;
    
public:
    vec() {
        this->d_ptr = new data_t[size];
        this->own_memory = true;
    }
    
    vec(data_t *d_ptr): d_ptr(d_ptr) {
        this->own_memory = false;
    }
    
    vec(std::vector<data_t> &&v) {
        assert(v.size() == size);
        this->d_ptr = new data_t[size];
        this->own_memory = true;
        for (int i = 0; i < v.size(); ++i) {
            d_ptr[i] = v[i];
        }
    }
    
    vec(const vec<size> & v) {
        this->d_ptr = new data_t[size];
        this->own_memory = true;
        for (int i = 0; i < size; ++i) {
            this->d_ptr[i] = v[i];
        }
    }
    
    vec(vec<size> && v) {
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
    
    inline data_t & operator [](const int i) const {
        assert(i < size);
        return *(d_ptr + i);
    }
};

DEFINE_VEC_ARITH_OPERATOR(add, +)
DEFINE_VEC_ARITH_OPERATOR(sub, -)
DEFINE_VEC_ARITH_OPERATOR(mul, *)
DEFINE_VEC_ARITH_OPERATOR(div, /)

#undef DEFINE_VEC_ARITH_METHOD
#undef DEFINE_VEC_ARITH_OPERATOR

#endif /* vec_h */
