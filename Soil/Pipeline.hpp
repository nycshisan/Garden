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
#include "Vertex.hpp"
#include "Fragment.hpp"
#include "Rasterizer.hpp"

#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <functional>

template <class Attribute, class Uniform>
class Pipeline {
private:
    unsigned int width, height;
    std::shared_ptr<WindowContext> wc;
    
    std::shared_ptr<Rasterizer> rasterizer;
    
public:
    std::vector<Attribute> vertex_buf;
    typename std::vector<Attribute>::iterator vertex_buf_iter = vertex_buf.begin();
    std::vector<int> index_buf;
    std::vector<int>::iterator index_buf_iter = index_buf.begin();
    Uniform uniform;
    
    std::function<std::shared_ptr<Vertex>(const Attribute &)> vertexShader;
    std::function<void(const Fragment &, const Uniform &, Pixel &)> fragmentShader;
    
    Pipeline(std::shared_ptr<WindowContext> wc): wc(wc) {
        this->rasterizer = std::make_shared<Rasterizer>(wc->width, wc->height);
    }
    
    void draw_rectangle() {
        std::shared_ptr<Vertex> v[4];
        for (int i = 0; i < 4; ++i) {
            v[i] = vertexShader(vertex_buf[i]);
            v[i]->convertToWindowCoord();
        }
        
        Fragment crtf;
        Pixel crtp;
        
        rasterizer->init(v);
        
        while (rasterizer->rasterize(crtf)) {
            fragmentShader(crtf, uniform, crtp);
            wc->setPixel(crtp);
        }        
    }
};

#endif /* Pipeline_hpp */
