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

class Vertex {
public:
    vec4 position;
    
    void convertToWindowCoord() {
        position.x = (position.x + 1.0) / 2;
        position.y = -(position.y - 1.0) / 2;
    }
};

#endif /* Vertex_h */
