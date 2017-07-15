//
//  main.cpp
//  Lab
//
//  Created by Nycshisan on 2017/7/1.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#include <cassert>
#include <random>
#include <ctime>

#include "Vec.hpp"
#include "Mat.hpp"

int main() {
    mat3 m = {{{2,2,12,0},
            {3,6,7,0},
            {1,9,4,0},
            {0,0,0,1}}};
    vec3 v = {1, 1, 1};
    vec3 r = m * v;
}
