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
    WindowContext *wc;
    
    Rasterizer *rasterizer;
    
    Fragment *frags;
    
public:
    std::vector<Attribute> vertex_buf;
    //typename std::vector<Attribute>::iterator vertex_buf_iter = vertex_buf.begin();
    std::vector<int> index_buf;
    //std::vector<int>::iterator index_buf_iter = index_buf.begin();
    Uniform uniform;
    
    std::function<void(const Attribute &, Vertex &)> vertexShader;
    std::function<void(const Fragment &, const Uniform &, vec4 &)> fragmentShader;
    
    Pipeline(WindowContext *wc): wc(wc) {
        this->frags = new Fragment[wc->width * wc->height];
        this->rasterizer = new Rasterizer(wc->width, wc->height, frags);
    }
    
    void draw_rectangle() {
        Vertex vertexes[4];
        for (size_t i = 0; i < 4; ++i) {
            vertexShader(vertex_buf[i], vertexes[i]);
            vertexes[i].convertToWindowCoord();
        }
        
        size_t frag_num = rasterizer->rasterize(vertexes);
        
        vec4 color;
        for (size_t i = 0; i < frag_num; ++i) {
            fragmentShader(frags[i], uniform, color);
            wc->setPixel(frags[i].position.x, frags[i].position.y, color);
        }
    }
    
    ~Pipeline() {
        delete rasterizer;
        delete[] frags;
    }
};

#endif /* Pipeline_hpp */
