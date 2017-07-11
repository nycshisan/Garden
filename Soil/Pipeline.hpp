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
    typedef Rasterizer<Varying> _Rasterizer;
    typedef Vertex<Varying> _Vertex;
    typedef Fragment<Varying> _Fragment;
    
    coord_t width, height;
    WindowContext *wc;
    
    _Rasterizer *rasterizer;
    
    _Fragment *frags;
    
    inline void makeVertex(typename std::vector<Attribute>::iterator &iter, _Vertex &v) {
        vertexShader(*(iter++), v);
        v.convertToWindowCoord();
    }
    
    void drawWithVertexPtrs(DrawType type, _Vertex **vertexPtrs);
    
public:
    data_t zClippingFrontSurface = NAN, zClippingBackSurface = NAN;
    
    std::vector<Attribute> vertexBuffer;
    typename std::vector<Attribute>::iterator vertexBufferIter;
    std::vector<int> indexBuffer;
    std::vector<int>::iterator indexBufferIter;
    Uniform uniform;
    
    std::function<void(const Attribute &, _Vertex &)> vertexShader;
    std::function<void(const _Fragment &, const Uniform &, vec4 &)> fragmentShader;
    
    Pipeline(WindowContext *wc): wc(wc) {
        this->width = wc->width; this->height = wc->height;
        this->frags = new _Fragment[width * height];
        this->rasterizer = new _Rasterizer(width, height, frags);
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
    
    _Vertex vertexPool[vertexNumber]; // Store the materials of vertexes
    _Vertex *vertexPtrs[vertexNumber]; // Store the pointers of vertexes for rasterizer
    
    // Index of vertex pointer to be filled for striping drawing.
    // Initialize it with -1 to slience warnings, but it will be really initialized before being used.
    size_t crtVertexPoolIndex = -1;
    
    // The pointer of the first vertex for LineLoop drawing
    _Vertex *firstVertex = nullptr;
    
    // Masks for TriangleFan drawing. Striping drawing should use these masks rather than the original arrays
    _Vertex *vertexPoolMask = vertexPool;
    _Vertex **vertexPtrsMask = vertexPtrs;
    
    // Setup
    vertexBufferIter = vertexBuffer.begin();
    if (isStrip) {
        // Set up the begining vertex pool for striping drawing
        if (isFan) {
            // The first vertex(centroid) are static, and the others are the same as striping drawing.
            makeVertex(vertexBufferIter, vertexPool[0]);
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
            // Store the first vertex information for LineLoop drawing.
            // The first vertex locates in the second(index 1) slot in the vertex pool
            firstVertex = new _Vertex(vertexPoolMask[1]);
        }

    } else {
        // For Point, Line and Triangle, vertex pointers are static
        for (size_t i = 0; i < vertexNumber; ++i) {
            vertexPtrs[i] = vertexPool + i;
        }
        // Buffer size must be a mutiple of vertex number
        assert(vertexBuffer.size() % vertexNumber == 0);
    }
    
    
    // Mainloop
    while (vertexBufferIter != vertexBuffer.end()) {
        // Make vertexes
        if (isStrip) {
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
        
        drawWithVertexPtrs(type, vertexPtrs);
    }
    
    // The last edge for LineLoop drawing
    if (isLoop) {
        // The pointer of the last vertex is always the last element of vertexPtrs
        _Vertex *lastVertex = vertexPtrsMask[vertexNumber - 1];
        
        _Vertex *lastVertexPtrs[vertexNumber];
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
void Pipeline<Attribute, Uniform, Varying>::drawWithVertexPtrs(DrawType type, _Vertex **vertexPtrs) {
    // Rasterize
    size_t fragNumber = rasterizer->rasterize(type, vertexPtrs);
    
    // Shade fragments
    _Fragment *fragEnd = frags + fragNumber;
    vec4 color;
    for (_Fragment *frag = frags; frag < fragEnd; ++frag) {
        coord_t pixelX = frag->pixelX, pixelY = frag->pixelY;
        // Clipping
        if (pixelX < 0 || pixelY < 0 || pixelX >= width || pixelY >= height) {
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
