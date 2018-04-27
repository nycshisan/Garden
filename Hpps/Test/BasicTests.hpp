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
    fragColor = uniform.color;
}

DEF_PREPARE {
    pipeline.vertexShader = VS;
    pipeline.fragmentShader = FS;
    
    ATTRIBUTE attr;
    
    attr.pos = {-0.95, 0.9, 0.0, 1.0};
    attr.size = 1;
    pipeline.attributeBuffer.emplace_back(attr);
    attr.pos = {-0.9, 0.9, 0.0, 1.0};
    attr.size = 3;
    pipeline.attributeBuffer.emplace_back(attr);
    attr.pos = {-0.85, 0.9, 0.0, 1.0};
    attr.size = 6;
    pipeline.attributeBuffer.emplace_back(attr);
    attr.pos = {0.8, 0.9, 0.0, 1.0};
    attr.size = 9;
    pipeline.attributeBuffer.emplace_back(attr);
}

DEF_DRAW {
    pipeline.uniform.color = Color::white;
    pipeline.drawArrays(Points, 0, 1);
    pipeline.uniform.color = Color::red;
    pipeline.drawArrays(Points, 1, 1);
    pipeline.uniform.color = Color::green;
    pipeline.drawArrays(Points, 2, 1);
    pipeline.uniform.color = Color::blue;
    pipeline.drawArrays(Points, 3, 1);
    
}

#undef TEST_NAME

// Test lines
#define TEST_NAME lines

DEF_ATTRIBUTE {
public:
    vec4 pos;
    vec4 color;
};

DEF_UNIFORM {};

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
    
    attr.pos = {-0.95, 0.5, 0.0, 1.0};
    attr.color = Color::red;
    pipeline.attributeBuffer.emplace_back(attr);
    attr.pos = {-0.85, 0.6, 0.0, 1.0};
    attr.color = Color::cyan;
    pipeline.attributeBuffer.emplace_back(attr);
}

DEF_DRAW {
    pipeline.drawArrays(Lines, 0, 2);
}

#undef TEST_NAME

// Test linestrip
#define TEST_NAME linestrip

DEF_ATTRIBUTE {
public:
    vec4 pos;
    vec4 color;
};

DEF_UNIFORM {};

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
    
    attr.pos = {-0.75, 0.5, 0.0, 1.0};
    attr.color = Color::red;
    pipeline.attributeBuffer.emplace_back(attr);
    attr.pos = {-0.65, 0.6, 0.0, 1.0};
    attr.color = Color::green;
    pipeline.attributeBuffer.emplace_back(attr);
    attr.pos = {-0.55, 0.5, 0.0, 1.0};
    attr.color = Color::blue;
    pipeline.attributeBuffer.emplace_back(attr);
}

DEF_DRAW {
    pipeline.drawArrays(LineStrip, 0, 3);
}

#undef TEST_NAME

// Test lineloop
#define TEST_NAME lineloop

DEF_ATTRIBUTE {
public:
    vec4 pos;
    vec4 color;
};

DEF_UNIFORM {};

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
    
    attr.pos = {-0.45, 0.5, 0.0, 1.0};
    attr.color = Color::red;
    pipeline.attributeBuffer.emplace_back(attr);
    attr.pos = {-0.35, 0.6, 0.0, 1.0};
    attr.color = Color::green;
    pipeline.attributeBuffer.emplace_back(attr);
    attr.pos = {-0.25, 0.5, 0.0, 1.0};
    attr.color = Color::blue;
    pipeline.attributeBuffer.emplace_back(attr);
}

DEF_DRAW {
    pipeline.drawArrays(LineLoop, 0, 3);
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
    
    attr.pos = {-0.9, -0.7, 0.0, 1.0};
    attr.color = Color::red;
    pipeline.attributeBuffer.emplace_back(attr);
    attr.pos = {-0.6, -0.3, 0.0, 1.0};
    attr.color = Color::green;
    pipeline.attributeBuffer.emplace_back(attr);
    attr.pos = {-0.5, -0.9, 0.0, 1.0};
    attr.color = Color::blue;
    pipeline.attributeBuffer.emplace_back(attr);
}

DEF_DRAW {
    pipeline.drawArrays(Triangles, 0, 3);
}

#undef TEST_NAME

// Test trianglestrip
#define TEST_NAME trianglestrip
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
    
    attr.pos = {-0.4, -0.7, 0.0, 1.0};
    attr.color = Color::red;
    pipeline.attributeBuffer.emplace_back(attr);
    attr.pos = {-0.1, -0.3, 0.0, 1.0};
    attr.color = Color::green;
    pipeline.attributeBuffer.emplace_back(attr);
    attr.pos = {0.0, -0.9, 0.0, 1.0};
    attr.color = Color::blue;
    pipeline.attributeBuffer.emplace_back(attr);
    attr.pos = {0.3, -0.4, 0.0, 1.0};
    attr.color = Color::red;
    pipeline.attributeBuffer.emplace_back(attr);
}

DEF_DRAW {
    pipeline.drawArrays(TriangleStrip, 0, 4);
}

#undef TEST_NAME

// Test trianglefan
#define TEST_NAME trianglefan
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
    
    attr.pos = {0.2, -0.7, 0.0, 1.0};
    attr.color = Color::red;
    pipeline.attributeBuffer.emplace_back(attr);
    attr.pos = {0.4, -0.3, 0.0, 1.0};
    attr.color = Color::green;
    pipeline.attributeBuffer.emplace_back(attr);
    attr.pos = {0.5, -0.9, 0.0, 1.0};
    attr.color = Color::blue;
    pipeline.attributeBuffer.emplace_back(attr);
    attr.pos = {0.8, -0.4, 0.0, 1.0};
    attr.color = Color::red;
    pipeline.attributeBuffer.emplace_back(attr);
}

DEF_DRAW {
    pipeline.drawArrays(TriangleFan, 0, 4);
}

#undef TEST_NAME

// Main
void _main(WindowContext *context) {
    Pipeline_points pipeline_points(context);
    Pipeline_lines pipeline_lines(context);
    Pipeline_linestrip pipeline_linestrip(context);
    Pipeline_lineloop pipeline_lineloop(context);
    Pipeline_triangles pipeline_triangles(context);
    Pipeline_trianglestrip pipeline_trianglestrip(context);
    Pipeline_trianglefan pipeline_trianglefan(context);
    
    prepare(pipeline_points);
    prepare(pipeline_lines);
    prepare(pipeline_linestrip);
    prepare(pipeline_lineloop);
    prepare(pipeline_triangles);
    prepare(pipeline_trianglestrip);
    prepare(pipeline_trianglefan);
    
    while (!context->windowWillClose()) {
        draw(pipeline_points);
        draw(pipeline_lines);
        draw(pipeline_linestrip);
        draw(pipeline_lineloop);
        draw(pipeline_triangles);
        draw(pipeline_trianglestrip);
        draw(pipeline_trianglefan);
        context->presentScene();
    }
}

#endif /* BasicTests_h */
