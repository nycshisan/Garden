//
//  Playground.cpp
//  Playground
//
//  Created by Nycshisan on 2017/4/27.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#include "Pipeline.hpp"

#include <memory>

class Attribute_HR {
public:
    vec4 pos;
};

class Uniform_HR {
    
};

void fs(const Fragment &f, const Uniform_HR &u, vec4 &color) {
    color = {1.0, 1.0, 1.0, 1.0};
}

void vs(const Attribute_HR &a, Vertex &v) {
    v.position = a.pos;
}

int main(int argc, const char *argv[]) {
    auto wc = new WindowContext(640, 480, "Garden");
    wc->setFPSBoundary(60.0);
    Pipeline<Attribute_HR, Uniform_HR> p(wc);
    p.fragmentShader = fs;
    p.vertexShader = vs;
    
    Attribute_HR attr1;
    attr1.pos = {-1.0, 1.0, 0.0, 1.0};
    p.vertex_buf.push_back(attr1);
    
    Attribute_HR attr2;
    attr2.pos = {-1.0, -1.0, 0.0, 1.0};
    p.vertex_buf.push_back(attr2);
    
    Attribute_HR attr3;
    attr3.pos = {1.0, -1.0, 0.0, 1.0};
    p.vertex_buf.push_back(attr3);
    
    Attribute_HR attr4;
    attr4.pos = {1.0, 1.0, 0.0, 1.0};
    p.vertex_buf.push_back(attr4);
    
    while (!wc->windowWillClose()) {
        for (size_t i = 0; i < 4; ++i) {
            for (size_t j = 0; j < 2; ++j) {
                p.vertex_buf[i].pos[j] *= 0.995;
            }
        }
        p.draw_rectangle();
        wc->presentScene();
    }
    
    delete wc;
}
