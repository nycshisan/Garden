//
//  Vertex.h
//  Garden
//
//  Created by Nycshisan on 2017/5/3.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef Vertex_h
#define Vertex_h

#include "vec.hpp"

class Vertex {
public:
    vec<4> position;
    
    Vertex(vec<4> position): position(position) {};
    
    void convertToWindowCoord() {
        position[0] = (position[0] + 1.0) / 2; // x
        position[1] = -(position[1] - 1.0) / 2; // y
    }
};

#endif /* Vertex_h */
