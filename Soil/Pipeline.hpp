//
//  Pipeline.hpp
//  Garden
//
//  Created by Nycshisan on 2017/4/28.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef Pipeline_hpp
#define Pipeline_hpp

#include "Helper.hpp"
#include "WindowContext.hpp"
#include "Vertex.hpp"
#include "Fragment.hpp"
#include "Rasterizer.hpp"

#include <vector>
#include <memory>
//#include <thread>
//#include <mutex>
#include <functional>

template <class Attribute, class Uniform, class Varying>
class Pipeline {
private:
    unsigned int width, height;
    WindowContext *wc;
    
    Rasterizer *rasterizer;
    
    Fragment *frags;
    
public:
    
    std::vector<Attribute> vertexBuffer;
    typename std::vector<Attribute>::iterator vertexBufferIter;
    std::vector<int> indexBuffer;
    std::vector<int>::iterator indexBufferIter;
    Uniform uniform;
    
    std::function<void(const Attribute &, Vertex &)> vertexShader;
    std::function<void(const Fragment &, const Uniform &, vec4 &)> fragmentShader;
    
    Pipeline(WindowContext *wc): wc(wc) {
        this->frags = new Fragment[wc->width * wc->height];
        this->rasterizer = new Rasterizer(wc->width, wc->height, frags);
    }
    
    void draw(drawType type);
    void drawElement(drawType type);
    
    ~Pipeline() {
        delete rasterizer;
        delete[] frags;
    }
};

template <class Attribute, class Uniform, class Varying>
void Pipeline<Attribute, Uniform, Varying>::draw(drawType type) {
    size_t vertexNumber;
    switch (type) {
        case Line:
            vertexNumber = 2;
            break;
        case Triangle:
            vertexNumber = 3;
            break;
            
        default:
            fatalError("Unknown draw type"); // Should never reach here
    }
    
    assert(vertexBuffer.size() >= vertexNumber);
    
    Vertex vertexPool[vertexNumber]; // Store the materials of vertexes
    Vertex *vertexPtrs[vertexNumber]; // Store the pointers of vertexes for rasterizer
    
    // Setup
    vertexBufferIter = vertexBuffer.begin();
    for (size_t i = 1; i < vertexNumber; ++i) {
        vertexShader(*(vertexBufferIter++), vertexPool[i]);
        vertexPtrs[i] = &vertexPool[i];
    }
    // At the beginging, the poll are fully materialized except the first slot
    size_t crtVertexPoolIndex = 0;
    
    // Mainloop
    while (vertexBufferIter != vertexBuffer.end()) {
        // L-shift the vertex pointers
        for (size_t i = 0; i < vertexNumber - 1; ++i) {
            vertexPtrs[i] = vertexPtrs[i + 1];
        }
        
        // Fill the last pointer
        vertexShader(*(vertexBufferIter++), vertexPool[crtVertexPoolIndex]);
        vertexPtrs[vertexNumber - 1] = vertexPool + crtVertexPoolIndex;
        crtVertexPoolIndex = (crtVertexPoolIndex + 1) % vertexNumber;
        
        // Rasterize
        size_t fragNumber = rasterizer->rasterize(type, vertexPtrs);
        
        // Shade fragments
        Fragment *fragEnd = frags + fragNumber;
        vec4 color;
        for (Fragment *frag = frags; frag < fragEnd; ++frag) {
            fragmentShader(*frag, uniform, color);
            wc->setPixel(frag->x, frag->y, color);
        }
    }
}

template <class Attribute, class Uniform, class Varying>
void Pipeline<Attribute, Uniform, Varying>::drawElement(drawType type) {
    fatalError("Unimplemented");
}

#endif /* Pipeline_hpp */
