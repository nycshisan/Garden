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
    
    void makeVertex(typename std::vector<Attribute>::iterator &iter, Vertex &v) {
        vertexShader(*(iter++), v);
        v.convertToWindowCoord();
    }
    
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
    bool isStrip = false, isFan = false, isLoop = false;
    switch (type) {
        case Points:
            vertexNumber = 1;
            break;
            
        case LineLoop:
            isLoop = true;
        case LineStrip:
            isStrip = true;
        case Lines:
            vertexNumber = 2;
            break;
            
        case TriangleFan:
            isFan = true;
        case TriangleStrip:
            isStrip = true;
        case Triangles:
            vertexNumber = 3;
            break;
            
        default:
            fatalError("Unknown draw type"); // Should never reach here
    }
    
    assert(vertexBuffer.size() >= vertexNumber && vertexBuffer.size() % vertexNumber == 0);
    
    Vertex vertexPool[vertexNumber]; // Store the materials of vertexes
    Vertex *vertexPtrs[vertexNumber]; // Store the pointers of vertexes for rasterizer
    
    size_t crtVertexPoolIndex; // Index of vertex pointer to be filled for striping drawing
    
    // Masks for TriangleFan drawing, striping draw should use these masks rather than the original arrays
    Vertex *vertexPoolMask = vertexPool;
    Vertex **vertexPtrsMask = vertexPtrs;
    
    // Setup
    vertexBufferIter = vertexBuffer.begin();
    if (isStrip) {
        // Set up the begining vertex pool for striping drawing
        if (isFan) {
            fatalError("unimplemented");
        }
        fatalError("unimplemented");
        // At the beginging, the poll are fully materialized except the first slot
        for (size_t i = 1; i < vertexNumber; ++i) {
            makeVertex(vertexBufferIter, vertexPoolMask[i]);
            vertexPtrsMask[i] = &vertexPoolMask[i];
        }
        crtVertexPoolIndex = 0;

    } else {
        // For Point, Line and Triangle, vertex pointers are static
        for (size_t i = 0; i < vertexNumber; ++i) {
            vertexPtrs[i] = vertexPool + i;
        }
    }
    
    
    // Mainloop
    while (vertexBufferIter != vertexBuffer.end()) {
        // Make vertexes
        if (isStrip) {
            fatalError("unimplemented");
            // L-shift the vertex pointers
            for (size_t i = 0; i < vertexNumber - 1; ++i) {
                vertexPtrsMask[i] = vertexPtrsMask[i + 1];
            }
            
            // Make new vertex and fill the last pointer
            makeVertex(vertexBufferIter, vertexPoolMask[crtVertexPoolIndex]);
            vertexPtrsMask[vertexNumber - 1] = vertexPoolMask + crtVertexPoolIndex;
            
            crtVertexPoolIndex = (crtVertexPoolIndex + 1) % vertexNumber;
        } else {
            // For Point, Line and Triangle, just get new vertexes
            for (size_t i = 0; i < vertexNumber; ++i) {
                makeVertex(vertexBufferIter, vertexPool[i]);
            }
        }
        
        // Rasterize
        size_t fragNumber = rasterizer->rasterize(type, vertexPtrs);
        
        // Shade fragments
        Fragment *fragEnd = frags + fragNumber;
        vec4 color;
        for (Fragment *frag = frags; frag < fragEnd; ++frag) {
            fragmentShader(*frag, uniform, color);
            wc->setPixel(frag->pixelX, frag->pixelY, color);
        }
    }
}

template <class Attribute, class Uniform, class Varying>
void Pipeline<Attribute, Uniform, Varying>::drawElement(drawType type) {
    fatalError("Unimplemented");
}

#endif /* Pipeline_hpp */
