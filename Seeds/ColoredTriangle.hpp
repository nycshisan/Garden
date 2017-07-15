//
//  ColoredTriangle.hpp
//  Garden
//
//  Created by Nycshisan on 2017/7/15.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef ColoredTriangle_h
#define ColoredTriangle_h

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
    
    data_t base = -0.3;
    attr.pos = {0.5, base, 0.0, 1.0};
    attr.color = {1.0, 0.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    attr.pos = {-0.5, base, 0.0, 1.0};
    attr.color = {0.0, 1.0, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    data_t upZ = base + 1.0 * sqrtf(3) / 2.0;
    attr.pos = {0.0, upZ, 0.0, 1.0};
    attr.color = {0.0, 0.0, 1.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
}

DRAW {
    pipeline.draw(Triangles);
}

#endif /* ColoredTriangle_h */
