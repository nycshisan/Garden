//
//  HelloElements.hpp
//  Garden
//
//  Created by Nycshisan on 2017/7/16.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef HelloElements_h
#define HelloElements_h

class Attribute {
public:
    vec4 pos;
    vec4 color;
};

class Uniform {
public:
    vec4 color = {1.0, 1.0, 1.0, 1.0};
};

VARYING_DEF_HELPER(Varying, vec4, color);

VERTEX_SHADER {
    v.position = a.pos;
    v.varying.color = a.color;
}

FRAGMENT_SHADER {
    color = f.varying.color;
}

PREPARE {
    Attribute attr;
    
    attr.pos = {-0.5, 0.5, 0.0, 1.0};
    attr.color = {1.0, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    attr.pos = {-0.5, -0.5, 0.0, 1.0};
    attr.color = {0.0, 1.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    attr.pos = {0.5, -0.5, 0.0, 1.0};
    attr.color = {0.0, 0.0, 1.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    attr.pos = {0.5, 0.5, 0.0, 1.0};
    attr.color = {1.0, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);

    int indexes[] = {1, 3, 0, 1, 3, 2};
    
    for (int index: indexes) {
        pipeline.indexBuffer.push_back(index);
    }
}

DRAW {
    pipeline.drawElements(Triangles);
}

#endif /* HelloElements_h */
