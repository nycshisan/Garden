//
//  Playground.cpp
//  Playground
//
//  Created by Nycshisan on 2017/4/27.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#include "Pipeline.hpp"

#include "HelloPoints.hpp"

int main(int argc, const char *argv[]) {
    auto wc = new WindowContext(640, 480, "Garden");
    wc->setFPSBoundary(60.0);
    Pipeline<Attribute, Uniform, Varying> pipeline(wc);
    pipeline.vertexShader = vertexShader;
    pipeline.fragmentShader = fragmentShader;
    
    prepare(pipeline);
    
    while (!wc->windowWillClose()) {
        pipeline.draw(DrawType);
        wc->presentScene();
    }
    
    delete wc;
}
