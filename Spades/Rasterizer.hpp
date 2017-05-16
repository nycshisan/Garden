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
    
    std::vector<Fragment> fragments;
    
public:
    Rasterizer(unsigned int width, unsigned int height): width(width), height(height) {}
    
    std::vector<Fragment> rasterize(std::shared_ptr<Vertex> v[]) {
        fragments.clear();
        
        for (int i = 0; i < 4; ++i) {
            v[i]->convertToWindowCoord();
        }
        
        for (int i = int(v[0]->position[0] * width); i < int(v[2]->position[0] * width); ++i) {
            for (int j = int(v[0]->position[1] * height); j < int(v[2]->position[1] * height); ++j) {
                fragments.emplace_back(vec<4>({data_t(i), data_t(j), 0.0, 1.0}));
            }
        }
        return fragments;
    }
    
};

#endif /* Rasterizer_h */
