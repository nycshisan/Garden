//
//  Playground.cpp
//  Playground
//
//  Created by Nycshisan on 2017/4/27.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#include "Pipeline.hpp"

#include <memory>

#define CRT_TASK HL

class Attribute_HL {
public:
    vec4 pos;
};

class Uniform_HL {
    
};

class Varying_HL {
    
};

void fs(const Fragment &f, const CrtUniform &u, vec4 &color) {
    color = {1.0, 1.0, 1.0, 1.0};
}

void vs(const CrtAttribute &a, Vertex &v) {
    v.position = a.pos;
}

int main(int argc, const char *argv[]) {
    auto wc = new WindowContext(640, 480, "Garden");
    wc->setFPSBoundary(60.0);
    Pipeline<CrtAttribute, CrtUniform, CrtVarying> p(wc);
    p.fragmentShader = fs;
    p.vertexShader = vs;
    
    CrtAttribute attr1;
    attr1.pos = {-1.0, 1.0, 0.0, 1.0};
    p.vertexBuffer.push_back(attr1);
    
    CrtAttribute attr2;
    attr2.pos = {-1.0, -1.0, 0.0, 1.0};
    p.vertexBuffer.push_back(attr2);
    
    CrtAttribute attr3;
    attr3.pos = {1.0, -1.0, 0.0, 1.0};
    p.vertexBuffer.push_back(attr3);
    
    CrtAttribute attr4;
    attr4.pos = {1.0, 1.0, 0.0, 1.0};
    p.vertexBuffer.push_back(attr4);
    
    while (!wc->windowWillClose()) {
//        for (size_t i = 0; i < 4; ++i) {
//            for (size_t j = 0; j < 2; ++j) {
//                p.vertexBuffer[i].pos[j] *= 0.995;
//            }
//        }
        p.draw_rectangle();
        wc->presentScene();
    }
    
    delete wc;
}
