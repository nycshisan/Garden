//
//  BasicTests.h
//  Garden
//
//  Created by Nycshisan on 2017/8/26.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef BasicTests_h
#define BasicTests_h

#include "Helper.hpp"

// Test points
#define TEST_NAME points

DEF_ATTRIBUTE {
public:
    vec4 pos;
    int size;
};

DEF_UNIFORM {
public:
    vec4 color;
};

#define Varying_points NullVarying

DEF_PIPELINE;

DEF_VS {
    vertex.position = attribute.pos;
    vertex.pointSize = attribute.size;
}
DEF_FS {
    fragColor = u.color;
}

DEF_PREPARE {
    pipeline.vertexShader = VS;
    pipeline.fragmentShader = FS;
    
    ATTRIBUTE attr;

    attr.pos = {-0.9, 0.9, 0.0, 1.0};
    attr.size = 10;
    
    pipeline.attributeBuffer.emplace_back(attr);
}

DEF_DRAW {
    
}

#undef TEST_NAME

// Test triangles
#define TEST_NAME triangles
DEF_ATTRIBUTE {
public:
    vec4 pos;
    vec4 color;
};

DEF_UNIFORM {
public:
    vec4 color = {1.0, 1.0, 1.0, 1.0};
};

VARYING_DEF_HELPER(VARYING, vec4, color);

DEF_PIPELINE;

DEF_VS {
    vertex.position = attribute.pos;
    vertex.varying.color = attribute.color;
}

DEF_FS {
    fragColor = frag.varying.color;
}

DEF_PREPARE {
    pipeline.vertexShader = VS;
    pipeline.fragmentShader = FS;
    
    ATTRIBUTE attr;
    
    data_t base = -0.3;
    attr.pos = {0.5, base + (float)0.1, 0.0, 1.0};
    attr.color = {1.0, 0.0, 0.0, 1.0};
    pipeline.attributeBuffer.push_back(attr);
    attr.pos = {-0.4, base - (float)0.1, 0.0, 1.0};
    attr.color = {0.0, 1.0, 0.0, 1.0};
    pipeline.attributeBuffer.push_back(attr);
    data_t upZ = base + 1.0 * sqrtf(3) / 2.0;
    attr.pos = {0.0, upZ, 0.0, 1.0};
    attr.color = {0.0, 0.0, 1.0, 1.0};
    pipeline.attributeBuffer.push_back(attr);
}

DEF_DRAW {
    pipeline.drawArrays(Triangles, 0, 3);
}

#undef TEST_NAME

// Main
void _main(WindowContext *context) {
    Pipeline_points pipeline_points(context);
    Pipeline_triangles pipeline_triangles(context);
    
    prepare(pipeline_points);
    prepare(pipeline_triangles);
    
    while (!context->windowWillClose()) {
        draw(pipeline_triangles);
        context->presentScene();
    }
}

#endif /* BasicTests_h */
