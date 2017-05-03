//
//  Playground.cpp
//  Playground
//
//  Created by Nycshisan on 2017/4/27.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#include "Pipeline.hpp"

#include "vec.hpp"

int main(int argc, const char * argv[]) {
    vec<1> vv = vec<1>({2});
    vec<1> v = vv + vec<1>({1}) + vec<1>({2}) + vec<1>({4}) + vv;
    printf("%f\n", v.dot(v));
}
