//
//  Vertex.h
//  Garden
//
//  Created by Nycshisan on 2017/5/3.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef Vertex_h
#define Vertex_h

#include "Varying.hpp"
#include "Vec.hpp"

class Vertex {
    
public:
    vec4 position;
    int pointSize = 1;
    
    data_t windowX, windowY;
    
    void convertToWindowCoord() {
        windowX = (position.x / position.w + 1.0) / 2;
        windowY = -(position.y / position.w - 1.0) / 2;
    }
};

#endif /* Vertex_h */
