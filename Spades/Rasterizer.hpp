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
    
    Fragment *frags;
    
public:
    Rasterizer(unsigned int width, unsigned int height, Fragment *frags): width(width), height(height), frags(frags) {}
    
    size_t rasterize(Vertex *vertexes) {
        size_t count = 0;
        size_t min_i = vertexes[0].position.x * width;
        size_t max_i = vertexes[2].position.x * width;
        size_t min_j = vertexes[0].position.y * height;
        size_t max_j = vertexes[2].position.y * height;
        
        Fragment *crt_frag = frags;
        for (size_t i = min_i; i < max_i; ++i) {
            for (size_t j = min_j; j < max_j; ++j) {
                crt_frag->x = i;
                crt_frag->y = j;
                ++count; ++crt_frag;
            }
        }
        
        return count;
    }
};

#endif /* Rasterizer_h */
