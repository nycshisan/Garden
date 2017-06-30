//
//  Vertex.h
//  Garden
//
//  Created by Nycshisan on 2017/5/3.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef Vertex_h
#define Vertex_h

#include "Vec.hpp"

template <class Varying>
class Vertex {
    template <class Attribute, class Uniform, class _Varying>
    friend class Pipeline;
    
    void convertToWindowCoord() {
        windowX = (position.x / position.w + 1.0) / 2;
        windowY = -(position.y / position.w - 1.0) / 2;
    }
    
public:
    vec4 position;
    int pointSize = 1;
    
    data_t windowX, windowY;
    
    Varying varying;
};

#endif /* Vertex_h */
