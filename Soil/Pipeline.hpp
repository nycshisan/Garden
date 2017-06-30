//
//  Pipeline.hpp
//  Garden
//
//  Created by Nycshisan on 2017/4/28.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef Pipeline_hpp
#define Pipeline_hpp

#include <vector>
#include <memory>
//#include <thread>
//#include <mutex>
#include <functional>

#include "WindowContext.hpp"
#include "Vertex.hpp"
#include "Fragment.hpp"
#include "Rasterizer.hpp"
#include "Misc.hpp"


template <class Attribute, class Uniform, class Varying>
class Pipeline {
private:
    unsigned int width, height;
    WindowContext *wc;
    
    Rasterizer<Varying> *rasterizer;
    
    Fragment<Varying> *frags;
    
    void makeVertex(typename std::vector<Attribute>::iterator &iter, Vertex<Varying> &v) {
        vertexShader(*(iter++), v);
        v.convertToWindowCoord();
    }
    
    void drawWithVertexPtrs(DrawType type, Vertex<Varying> **vertexPtrs);
    
public:
    data_t zClippingFrontSurface = NAN, zClippingBackSurface = NAN;
    
    std::vector<Attribute> vertexBuffer;
    typename std::vector<Attribute>::iterator vertexBufferIter;
    std::vector<int> indexBuffer;
    std::vector<int>::iterator indexBufferIter;
    Uniform uniform;
    
    std::function<void(const Attribute &, Vertex<Varying> &)> vertexShader;
    std::function<void(const Fragment<Varying> &, const Uniform &, vec4 &)> fragmentShader;
    
    Pipeline(WindowContext *wc): wc(wc) {
        this->width = wc->width; this->height = wc->height;
        this->frags = new Fragment<Varying>[width * height];
        this->rasterizer = new Rasterizer<Varying>(width, height, frags);
    }
    
    void draw(DrawType type);
    void drawElement(DrawType type);
    
    ~Pipeline() {
        delete rasterizer;
        delete[] frags;
    }
};

template <class Attribute, class Uniform, class Varying>
void Pipeline<Attribute, Uniform, Varying>::draw(DrawType type) {
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
    
    assert(vertexBuffer.size() >= vertexNumber); // Check buffer size
    
    Vertex<Varying> vertexPool[vertexNumber]; // Store the materials of vertexes
    Vertex<Varying> *vertexPtrs[vertexNumber]; // Store the pointers of vertexes for rasterizer
    
    // Index of Vertex<Varying> pointer to be filled for striping drawing.
    // Initialize it with -1 to slience warnings, but it will be really initialized before being used.
    size_t crtVertexPoolIndex = -1;
    
    // The pointer of the first Vertex<Varying> for LineLoop drawing
    Vertex<Varying> *firstVertex = nullptr;
    
    // Masks for TriangleFan drawing. Striping drawing should use these masks rather than the original arrays
    Vertex<Varying> *vertexPoolMask = vertexPool;
    Vertex<Varying> **vertexPtrsMask = vertexPtrs;
    
    // Setup
    vertexBufferIter = vertexBuffer.begin();
    if (isStrip) {
        // Set up the begining Vertex<Varying> pool for striping drawing
        if (isFan) {
            fatalError("unimplemented");
            // The first vertex(centroid) are static, and the others are the same as striping drawing.
            --vertexNumber;
            ++vertexPoolMask;
            ++vertexPtrsMask;
        }
        
        // At the beginging, the poll are fully materialized except the first slot
        for (size_t i = 1; i < vertexNumber; ++i) {
            makeVertex(vertexBufferIter, vertexPoolMask[i]);
            vertexPtrsMask[i] = &vertexPoolMask[i];
        }
        crtVertexPoolIndex = 0;
        
        if (isLoop) {
            // Store the first Vertex<Varying> information for LineLoop drawing.
            // Now the first Vertex<Varying> locates in the second(index 1) slot in the Vertex<Varying> pool
            firstVertex = new Vertex<Varying>(vertexPoolMask[1]);
        }

    } else {
        // For Point, Line and Triangle, Vertex<Varying> pointers are static
        for (size_t i = 0; i < vertexNumber; ++i) {
            vertexPtrs[i] = vertexPool + i;
        }
        // Buffer size must be a mutiple of Vertex<Varying> number
        assert(vertexBuffer.size() % vertexNumber == 0);
    }
    
    
    // Mainloop
    while (vertexBufferIter != vertexBuffer.end()) {
        // Make vertexes
        if (isStrip) {
            // L-shift the Vertex<Varying> pointers
            for (size_t i = 0; i < vertexNumber - 1; ++i) {
                vertexPtrsMask[i] = vertexPtrsMask[i + 1];
            }
            
            // Make new Vertex<Varying> and fill the last pointer
            makeVertex(vertexBufferIter, vertexPoolMask[crtVertexPoolIndex]);
            vertexPtrsMask[vertexNumber - 1] = vertexPoolMask + crtVertexPoolIndex;
            
            crtVertexPoolIndex = (crtVertexPoolIndex + 1) % vertexNumber;
        } else {
            // For Point, Line and Triangle, just get new vertexes
            for (size_t i = 0; i < vertexNumber; ++i) {
                makeVertex(vertexBufferIter, vertexPool[i]);
            }
        }
        
        drawWithVertexPtrs(type, vertexPtrs);
    }
    
    // The last edge for LineLoop drawing
    if (isLoop) {
        // The pointer of the last Vertex<Varying> is always the last element of vertexPtrs
        Vertex<Varying> *lastVertex = vertexPtrsMask[vertexNumber - 1];
        
        Vertex<Varying> *lastVertexPtrs[vertexNumber];
        lastVertexPtrs[0] = lastVertex; lastVertexPtrs[1] = firstVertex;
        
        drawWithVertexPtrs(type, lastVertexPtrs);
        
        delete firstVertex;
    }
}

template <class Attribute, class Uniform, class Varying>
void Pipeline<Attribute, Uniform, Varying>::drawElement(DrawType type) {
    fatalError("Unimplemented");
}

template <class Attribute, class Uniform, class Varying>
void Pipeline<Attribute, Uniform, Varying>::drawWithVertexPtrs(DrawType type, Vertex<Varying> **vertexPtrs) {
    // Rasterize
    size_t fragNumber = rasterizer->rasterize(type, vertexPtrs);
    
    // Shade fragments
    Fragment<Varying> *fragEnd = frags + fragNumber;
    vec4 color;
    for (Fragment<Varying> *frag = frags; frag < fragEnd; ++frag) {
        coord_t pixelX = frag->pixelX, pixelY = frag->pixelY;
        // Clipping
        if (pixelX >= width || pixelY >= height) {
            continue;
        }
        if (!isnan(zClippingFrontSurface) && frag->fragZ > zClippingFrontSurface) {
            continue;
        }
        if (!isnan(zClippingBackSurface) && frag->fragZ < zClippingFrontSurface) {
            continue;
        }
        
        fragmentShader(*frag, uniform, color);
        wc->setPixel(pixelX, pixelY, color);
    }
}

#endif /* Pipeline_hpp */
