//
//  Pipeline.hpp
//  Garden
//
//  Created by Nycshisan on 2017/4/28.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef Pipeline_hpp
#define Pipeline_hpp

#include "WindowContext.hpp"
#include "Resource.hpp"
#include "Vertex.hpp"
#include "Rasterizer.hpp"

class Pipeline {
private:
    WindowContext *wc;
    
    int vertexShader;
    int fragmentShader;
    Rasterizer rasterizer;
    int attributes;
    int uniforms;
    
public:
    Pipeline() {
        this->wc = new WindowContext(640, 480, "Garden");
    }
    
    ~Pipeline() {
        delete this->wc;
    }
};

#endif /* Pipeline_hpp */
