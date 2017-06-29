//
//  Playground.cpp
//  Playground
//
//  Created by Nycshisan on 2017/4/27.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#include "Pipeline.hpp"

//#include "HelloPoints.hpp"
//#include "HelloLines.hpp"
#include "LineStripAndLineLoop.hpp"

int main(int argc, const char *argv[]) {
    auto wc = new WindowContext(600, 600, "Garden");
    wc->setFPSBoundary(60.0);
    Pipeline<Attribute, Uniform> pipeline(wc);
    pipeline.vertexShader = vertexShader;
    pipeline.fragmentShader = fragmentShader;
    
    prepare(pipeline);
    
    while (!wc->windowWillClose()) {
        draw(pipeline);
        wc->presentScene();
    }
    
    delete wc;
}
