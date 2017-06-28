//
//  HelloPoints.h
//  Garden
//
//  Created by Nycshisan on 2017/6/27.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef HelloPoints_h
#define HelloPoints_h

#define DrawType Points

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
    v.pointSize = 100;
}

void fragmentShader(const Fragment &f, const Uniform &u, vec4 &color) {
    color = {1.0, 1.0, 1.0, 1.0};
}

void prepare(Pipeline<Attribute, Uniform, Varying> &pipeline) {
    Attribute attr1;
    attr1.pos = {-0.5, 0.5, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr1);
    
    Attribute attr2;
    attr2.pos = {0.5, -0.5, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr2);
}

#endif /* HelloPoints_h */
