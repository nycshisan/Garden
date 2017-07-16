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
#include "BufferCursor.hpp"
#include "Vertex.hpp"
#include "Fragment.hpp"
#include "Rasterizer.hpp"
#include "Misc.hpp"

#define MAX_VERTEX_NUM 3

template <class Attribute, class Uniform, class Varying>
class Pipeline {
    typedef Rasterizer<Varying> _Rasterizer;
    typedef Vertex<Varying> _Vertex;
    typedef Fragment<Varying> _Fragment;
    
    coord_t width, height;
    WindowContext *wc;
    
    _Rasterizer *rasterizer;
    
    // Fragment pool, will be filled by the rasterizer
    _Fragment *frags;
    
    // Vertex pool, will be filled in the `drawVertexes` step and be used in the `drawFragments` step
    _Vertex vertexPool[MAX_VERTEX_NUM]; // Store the materials of vertexes
    _Vertex *vertexPtrs[MAX_VERTEX_NUM]; // Store the pointers of vertexes for rasterizer
    
    // Current draw type
    DrawType type;
    
    // Cursor for visiting vertex buffer and index buffer
    BufferCursor<Attribute> cursor = BufferCursor<Attribute>(vertexBuffer, indexBuffer);
    
    // This function will materialize the passed vertex, and increase the buffer cursor
    ALWAYS_INLINE void _makeVertex(_Vertex &v) {
        vertexShader(*cursor, v);
        ++cursor;
        v.convertToWindowCoord();
    }
    
    void drawVertexes();
    void drawFragments();
    
    GLfloat *zBuffer;
    
public:
    data_t zClippingFrontSurface = NAN, zClippingBackSurface = NAN;
    
    std::vector<Attribute> vertexBuffer;
    std::vector<size_t> indexBuffer;
    Uniform uniform;
    
    std::function<void(const Attribute &, _Vertex &)> vertexShader;
    std::function<void(const _Fragment &, const Uniform &, vec4 &)> fragmentShader;
    
    Pipeline(WindowContext *wc): wc(wc) {
        this->width = wc->width; this->height = wc->height;
        this->frags = new _Fragment[width * height];
        this->rasterizer = new _Rasterizer(width, height, frags);
        this->zBuffer = wc->zBuffer;
    }
    
    void draw(DrawType type);
    void drawElements(DrawType type);
    
    ~Pipeline() {
        delete rasterizer;
        delete[] frags;
    }
};

template <class Attribute, class Uniform, class Varying>
void Pipeline<Attribute, Uniform, Varying>::drawVertexes() {
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
    
    assert(cursor.size() >= vertexNumber); // Check buffer size
    
    // Index of vertex pointer to be filled for striping drawing.
    // Initialize it with -1 to slience warnings, but it will be really initialized before being used.
    size_t crtVertexPoolIndex = -1;
    
    // The pointer of the first vertex for LineLoop drawing
    _Vertex *firstVertex = nullptr;
    
    // Masks for TriangleFan drawing. Striping drawing should use these masks rather than the original arrays
    _Vertex *vertexPoolMask = vertexPool;
    _Vertex **vertexPtrsMask = vertexPtrs;
    
    // Setup
    cursor.begin();
    if (isStrip) {
        // Set up the begining vertex pool for striping drawing
        if (isFan) {
            // The first vertex(centroid) are static, and the others are the same as striping drawing.
            _makeVertex(vertexPool[0]);
            --vertexNumber;
            ++vertexPoolMask;
            ++vertexPtrsMask;
        }
        
        // At the beginging, the poll are fully materialized except the first slot
        for (size_t i = 1; i < vertexNumber; ++i) {
            _makeVertex(vertexPoolMask[i]);
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
        assert(cursor.size() % vertexNumber == 0);
    }
    
    
    // Mainloop
    while (!cursor.end()) {
        // Make vertexes
        if (isStrip) {
            // L-shift the vertex pointers
            for (size_t i = 0; i < vertexNumber - 1; ++i) {
                vertexPtrsMask[i] = vertexPtrsMask[i + 1];
            }
            
            // Make new vertex and fill the last pointer
            _makeVertex(vertexPoolMask[crtVertexPoolIndex]);
            vertexPtrsMask[vertexNumber - 1] = vertexPoolMask + crtVertexPoolIndex;
            
            crtVertexPoolIndex = (crtVertexPoolIndex + 1) % vertexNumber;
        } else {
            // For Point, Line and Triangle, just get new vertexes
            for (size_t i = 0; i < vertexNumber; ++i) {
                _makeVertex(vertexPool[i]);
            }
        }
        
        drawFragments();
    }
    
    // The last edge for LineLoop drawing
    if (isLoop) {
        // The pointer of the last vertex is always the last element of vertexPtrs
        _Vertex *lastVertex = vertexPtrsMask[vertexNumber - 1];
        
        vertexPtrs[0] = lastVertex; vertexPtrs[1] = firstVertex;
        
        drawFragments();
        
        delete firstVertex;
    }
}

template <class Attribute, class Uniform, class Varying>
void Pipeline<Attribute, Uniform, Varying>::drawFragments() {
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
        
        // Z-test
        if (wc->enabledZTest) {
            frag->normalizeZ();
            GLfloat *zBufPtr = zBuffer + pixelX * width + pixelY;
            if (frag->normalizedZ <= *zBufPtr) {
                continue;
            } else {
                *zBufPtr = frag->normalizedZ;
            }
        }
        
        fragmentShader(*frag, uniform, color);
        wc->setPixel(pixelX, pixelY, color);
    }
}

template <class Attribute, class Uniform, class Varying>
void Pipeline<Attribute, Uniform, Varying>::draw(DrawType type) {
    this->type = type;
    cursor.type = BufferCursor<Attribute>::BufferCursorType::Direct;
    drawVertexes();
}

template <class Attribute, class Uniform, class Varying>
void Pipeline<Attribute, Uniform, Varying>::drawElements(DrawType type) {
    this->type = type;
    cursor.type = BufferCursor<Attribute>::BufferCursorType::Element;
    drawVertexes();
}

#endif /* Pipeline_hpp */
