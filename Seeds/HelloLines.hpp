//
//  HelloLine.hpp
//  Garden
//
//  Created by Nycshisan on 2017/6/27.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef HelloLines_h
#define HelloLines_h

#define CrtDrawType Lines

class AttributeHelloLines {
public:
    vec4 pos;
};

class UniformHelloLines {
    
};

class VaryingHelloLines {
    
};

void fsHelloLines(const Fragment &f, const CrtUniform &u, vec4 &color) {
    color = {1.0, 1.0, 1.0, 1.0};
}

void vsHelloLines(const CrtAttribute &a, Vertex &v) {
    v.position = a.pos;
    v.pointSize = 100;
}

void prepare(Pipeline<CrtAttribute, CrtUniform, CrtVarying> &pipeline) {
    CrtAttribute attr1;
    attr1.pos = {-0.5, 0.5, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr1);
    
    CrtAttribute attr2;
    attr2.pos = {0.5, -0.5, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr2);
}

#endif /* HelloLines_h */
