//
//  Vertex.h
//  Garden
//
//  Created by Nycshisan on 2017/5/3.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef Vertex_h
#define Vertex_h

#include <cmath>

#include "Vec.hpp"
#include "Mat.hpp"

template <class Varying>
class Vertex {
    template <class Attribute, class Uniform, class _Varying>
    friend class Pipeline;
    
    bool _screenMapped = false;
    
    ALWAYS_INLINE void screenMap(coord_t width, coord_t height) {
        if (!_screenMapped) {
            _screenMapped = true;
            
            data_t windowX = (position.x / position.w + 1.0) / 2;
            data_t windowY = -(position.y / position.w - 1.0) / 2;
            pixelX = (coord_t)std::round(windowX * width);
            pixelY = (coord_t)std::round(windowY * height);
        }
    }
    
public:
    vec4 position;
    int pointSize = 1;
    
    coord_t pixelX, pixelY;
    
    Varying varying;
};

#endif /* Vertex_h */
