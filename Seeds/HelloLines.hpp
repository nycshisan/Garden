//
//  HelloLine.hpp
//  Garden
//
//  Created by Nycshisan on 2017/6/27.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef HelloLines_h
#define HelloLines_h

#define DrawType Lines

class Attribute {
public:
    vec4 pos;
};

class Uniform {
    
};

class Varying {
    
};

void vertexShader(const Attribute &a, Vertex &v) {
    v.position = a.pos;
}

void fragmentShader(const Fragment &f, const Uniform &u, vec4 &color) {
    color = {1.0, 1.0, 1.0, 1.0};
}

void prepare(Pipeline<Attribute, Uniform, Varying> &pipeline) {
    Attribute attr;
    
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
    
    attr.pos = {0.0, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    attr.pos = {0.0, -0.5, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    
    attr.pos = {0.0, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    attr.pos = {-0.5, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    
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
    
    attr.pos = {0.0, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    attr.pos = {0.0, 0.5, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    
    attr.pos = {0.0, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    attr.pos = {-0.5, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    
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
    
    attr.pos = {0.0, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    attr.pos = {0.0, -0.5, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    
    attr.pos = {0.0, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    attr.pos = {0.5, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
}

#endif /* HelloLines_h */
