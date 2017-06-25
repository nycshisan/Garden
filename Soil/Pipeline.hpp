//
//  Pipeline.hpp
//  Garden
//
//  Created by Nycshisan on 2017/4/28.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef Pipeline_hpp
#define Pipeline_hpp

#include "Macro.hpp"
#include "WindowContext.hpp"
#include "Vertex.hpp"
#include "Fragment.hpp"
#include "Rasterizer.hpp"

#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <functional>

template <class Attribute, class Uniform, class Varying>
class Pipeline {
private:
    unsigned int width, height;
    WindowContext *wc;
    
    Rasterizer *rasterizer;
    
    Fragment *frags;
    
public:
    enum drawType {
        line,
        rectangle,
        triangle
    };
    
    std::vector<Attribute> vertexBuffer;
    typename std::vector<Attribute>::iterator vertexBufferIter = vertexBuffer.begin();
    std::vector<int> indexBuffer;
    std::vector<int>::iterator indexBufferIter = indexBuffer.begin();
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
            vertexShader(vertexBuffer[i], vertexes[i]);
            vertexes[i].convertToWindowCoord();
        }
        
        size_t frag_num = rasterizer->rasterize(vertexes);
        Fragment *frag_end = frags + frag_num;
        
        vec4 color;
        for (Fragment *frag = frags; frag < frag_end; ++frag) {
            fragmentShader(*frag, uniform, color);
            wc->setPixel(frag->x, frag->y, color);
        }
    }
    
    void draw(drawType type);
    
    ~Pipeline() {
        delete rasterizer;
        delete[] frags;
    }
};

template <class Attribute, class Uniform, class Varying>
void Pipeline<Attribute, Uniform, Varying>::draw(drawType type) {
    size_t vertexesNumber;
}

#endif /* Pipeline_hpp */
