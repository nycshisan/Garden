//
//  ClippingAndZTest.hpp
//  Garden
//
//  Created by Nycshisan on 2017/6/28.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef ClippingAndZTest_h
#define ClippingAndZTest_h

class Attribute {
public:
    vec4 pos;
};

class Uniform {
public:
    vec4 color;
};

#define Varying NullVarying

VERTEX_SHADER {
    v.position = a.pos;
}

FRAGMENT_SHADER {
    color = u.color;
}

PREPARE {
    Attribute attr;
    data_t base, upZ;
    
    base = -0.3;
    attr.pos = {0.5, base, 1.0, 1.0};
    pipeline.attributeBuffer.push_back(attr);
    attr.pos = {-0.5, base, 1.0, 1.0};
    pipeline.attributeBuffer.push_back(attr);
    upZ = base + 1.0 * sqrtf(3) / 2.0;
    attr.pos = {0.0, upZ, 1.0, 1.0};
    pipeline.attributeBuffer.push_back(attr);
    
    base = -0.1;
    attr.pos = {0.5, base, 0.5, 1.0};
    pipeline.attributeBuffer.push_back(attr);
    attr.pos = {-0.5, base, 0.5, 1.0};
    pipeline.attributeBuffer.push_back(attr);
    upZ = base + 1.0 * sqrtf(3) / 2.0;
    attr.pos = {0.0, upZ, 0.5, 1.0};
    pipeline.attributeBuffer.push_back(attr);
    
    base = 0.1;
    attr.pos = {0.5, base, 0.0, 1.0};
    pipeline.attributeBuffer.push_back(attr);
    attr.pos = {-0.5, base, 0.0, 1.0};
    pipeline.attributeBuffer.push_back(attr);
    upZ = base + 1.0 * sqrtf(3) / 2.0;
    attr.pos = {0.0, upZ, 0.0, 1.0};
    pipeline.attributeBuffer.push_back(attr);
}

DRAW {
    pipeline.uniform.color = {1.0, 0.0, 0.0, 1.0};
    pipeline.indexBuffer = {0, 1, 2};
    pipeline.drawElements(Triangles);
    pipeline.uniform.color = {0.0, 1.0, 0.0, 1.0};
    pipeline.indexBuffer = {3, 4, 5};
    pipeline.drawElements(Triangles);
    pipeline.uniform.color = {0.0, 0.0, 1.0, 1.0};
    pipeline.indexBuffer = {6, 7, 8};
    pipeline.drawElements(Triangles);
}

#endif /* ClippingAndZTest_h */
