//
//  Playground.cpp
//  Playground
//
//  Created by Nycshisan on 2017/4/27.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#include "Pipeline.hpp"

#include <memory>

#define CRT_TASK HelloPoint

#define CrtAttribute CONCAT(Attribute, CRT_TASK)
#define CrtUniform CONCAT(Uniform, CRT_TASK)
#define CrtVarying CONCAT(Varying, CRT_TASK)

class AttributeHelloPoint {
public:
    vec4 pos;
};

class UniformHelloPoint {
    
};

class VaryingHelloPoint {
    
};

void fs(const Fragment &f, const CrtUniform &u, vec4 &color) {
    color = {1.0, 1.0, 1.0, 1.0};
}

void vs(const CrtAttribute &a, Vertex &v) {
    v.position = a.pos;
    v.pointSize = 100;
}

int main(int argc, const char *argv[]) {
    auto wc = new WindowContext(640, 480, "Garden");
    wc->setFPSBoundary(60.0);
    Pipeline<CrtAttribute, CrtUniform, CrtVarying> p(wc);
    p.fragmentShader = fs;
    p.vertexShader = vs;
    
    CrtAttribute attr1;
    attr1.pos = {-0.5, 0.5, 0.0, 1.0};
    p.vertexBuffer.push_back(attr1);
    
    CrtAttribute attr2;
    attr2.pos = {0.5, -0.5, 0.0, 1.0};
    p.vertexBuffer.push_back(attr2);
    
    
    while (!wc->windowWillClose()) {
        p.draw(Point);
        wc->presentScene();
    }
    
    delete wc;
}
