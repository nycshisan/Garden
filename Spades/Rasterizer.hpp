//
//  Rasterizer.hpp
//  Garden
//
//  Created by Nycshisan on 2017/5/3.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef Rasterizer_h
#define Rasterizer_h

#include <vector>
#include <memory>

#include "Fragment.hpp"

class Rasterizer {
    unsigned int width, height;
    
    int i, j, min_i, max_i, min_j, max_j;
    
public:
    Rasterizer(unsigned int width, unsigned int height): width(width), height(height) {}
    
    void init(std::shared_ptr<Vertex> v[]) {
        min_i = int(v[0]->position[0] * width);
        max_i = int(v[2]->position[0] * width);
        min_j = int(v[0]->position[1] * height);
        max_j = int(v[2]->position[1] * height);
        i = min_i;
        j = min_j;
    }
    
    bool rasterize(Fragment &f) {
        if (j >= max_j) {
            ++i;
            j = min_j;
        }
        if (i >= max_i) {
            return false;
        }
        f.position[0] = data_t(i);
        f.position[1] = data_t(j);
        f.position[2] = 0.0;
        f.position[3] = 1.0;
        ++j;
        return true;
    }
};

#endif /* Rasterizer_h */
