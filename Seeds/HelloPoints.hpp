//
//  HelloPoints.h
//  Garden
//
//  Created by Nycshisan on 2017/6/27.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef HelloPoints_h
#define HelloPoints_h

class Attribute {
public:
    vec4 pos;
};

class Uniform {};

VERTEX_SHADER {
    v.position = a.pos;
    v.pointSize = 100;
}

FRAGMENT_SHADER {
    color = {1.0, 1.0, 1.0, 1.0};
}

PREPARE {
    Attribute attr;
    
    attr.pos = {-0.5, 0.5, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
    attr.pos = {0.5, -0.5, 0.0, 1.0};
    pipeline.vertexBuffer.push_back(attr);
}

DRAW {
    pipeline.draw(Points);
}

#endif /* HelloPoints_h */
