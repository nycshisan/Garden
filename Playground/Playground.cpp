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
    vec<4> pos;
};

class Uniform_HR {
    
};

Pixel fs(const Fragment &f, const Uniform_HR &u) {
    std::pair<int, int> xy(int(f.position[0]), int(f.position[1]));
    return Pixel(xy, vec<4>({1.0, 1.0, 1.0, 1.0}));
}

std::shared_ptr<Vertex> vs(const Attribute_HR &a) {
    auto v = std::make_shared<Vertex>(a.pos);
    return v;
}



int main(int argc, const char *argv[]) {
    auto wc = std::make_shared<WindowContext>(640, 480, "Garden");
    wc->setFPSBoundary(60.0);
    Pipeline<Attribute_HR, Uniform_HR> p(wc);
    p.fragmentShader = fs;
    p.vertexShader = vs;
    
    Attribute_HR attr1;
    attr1.pos = vec<4>({-1.0, 1.0, 0.0, 1.0});
    p.vertex_buf.push_back(attr1);
    
    Attribute_HR attr2;
    attr2.pos = vec<4>({-1.0, -1.0, 0.0, 1.0});
    p.vertex_buf.push_back(attr2);
    
    Attribute_HR attr3;
    attr3.pos = vec<4>({1.0, -1.0, 0.0, 1.0});
    p.vertex_buf.push_back(attr3);
    
    Attribute_HR attr4;
    attr4.pos = vec<4>({1.0, 1.0, 0.0, 1.0});
    p.vertex_buf.push_back(attr4);
    
    while (!wc->windowWillClose()) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 2; ++j) {
                p.vertex_buf[i].pos[j] *= 0.99;
            }
        }
        p.draw_rectangle();
        wc->presentScene();
    }
}
