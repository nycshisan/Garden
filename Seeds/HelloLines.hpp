//
//  HelloLine.hpp
//  Garden
//
//  Created by Nycshisan on 2017/6/27.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef HelloLines_h
#define HelloLines_h

class Attribute {
public:
    vec4 pos;
};

class Uniform {};

VERTEX_SHADER {
    v.position = a.pos;
}

FRAGMENT_SHADER {
    color = {1.0, 1.0, 1.0, 1.0};
}

PREPARE {
    Attribute attr;
    
    // Axes
    attr.pos = {0.0, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    attr.pos = {0.0, 0.5, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    
    attr.pos = {0.0, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    attr.pos = {0.5, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    
    attr.pos = {0.0, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    attr.pos = {0.0, -0.5, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    
    attr.pos = {0.0, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    attr.pos = {-0.5, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    
    // First quadrant
    attr.pos = {0.0, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    attr.pos = {0.5, 0.5, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    
    attr.pos = {0.0, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    attr.pos = {0.5, 0.25, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    
    attr.pos = {0.0, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    attr.pos = {0.25, 0.5, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    
    // Second quadrant
    attr.pos = {0.0, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    attr.pos = {-0.5, 0.5, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    
    attr.pos = {0.0, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    attr.pos = {-0.5, 0.25, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    
    attr.pos = {0.0, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    attr.pos = {-0.25, 0.5, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    
    // Third quadrant
    attr.pos = {0.0, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    attr.pos = {-0.5, -0.5, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    
    attr.pos = {0.0, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    attr.pos = {-0.5, -0.25, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    
    attr.pos = {0.0, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    attr.pos = {-0.25, -0.5, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    
    // Fourth quadrant
    attr.pos = {0.0, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    attr.pos = {0.5, -0.5, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    
    attr.pos = {0.0, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    attr.pos = {0.5, -0.25, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    
    attr.pos = {0.0, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    attr.pos = {0.25, -0.5, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
}

DRAW {
    pipeline.draw(Lines);
}

#endif /* HelloLines_h */
