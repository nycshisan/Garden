//
//  Garden.cpp
//  Garden
//
//  Created by Nycshisan on 2017/4/27.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#include "Pipeline.hpp"

#include "BasicTests.hpp"

int main(int argc, const char *argv[]) {    
    auto context = new WindowContext(600, 480, "Garden");
    context->setFPSBoundary(60.0);
    _main(context);
    
    delete context;
}
