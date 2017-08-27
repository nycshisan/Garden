//
//  LineStripAndLineLoop.hpp
//  Garden
//
//  Created by Nycshisan on 2017/6/28.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef LineStripAndLineLoop_h
#define LineStripAndLineLoop_h

class Attribute {
public:
    vec4 pos;
};

class Uniform {};

#define Varying NullVarying

VERTEX_SHADER {
    v.position = a.pos;
}

FRAGMENT_SHADER {
    color = {1.0, 1.0, 1.0, 1.0};
}

PREPARE {
    Attribute attr;
    
    data_t base = -0.3;
    attr.pos = {0.5, base, 0.0, 1.0};
    pipeline.attributeBuffer.push_back(attr);
    attr.pos = {-0.5, base, 0.0, 1.0};
    pipeline.attributeBuffer.push_back(attr);
    data_t upZ = base + 1.0 * sqrtf(3) / 2.0;
    attr.pos = {0.0, upZ, 0.0, 1.0};
    pipeline.attributeBuffer.push_back(attr);
}

DRAW {
    pipeline.draw(LineLoop);
}

#endif /* LineStripAndLineLoop_h */