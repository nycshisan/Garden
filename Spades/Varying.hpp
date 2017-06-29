//
//  Varying.hpp
//  Garden
//
//  Created by Nycshisan on 2017/6/29.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef Varying_h
#define Varying_h

#include <map>

#include "Vec.hpp"

union VaryingData {
    int i;
    data_t f;
    vec2 v2;
    vec3 v3;
    vec4 v4;
};

typedef std::map<std::string, VaryingData> Varying;

#endif /* Varying_h */
