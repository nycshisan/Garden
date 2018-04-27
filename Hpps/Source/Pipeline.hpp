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
#include "Buffer.hpp"
#include "BufferCursor.hpp"
#include "Vertex.hpp"
#include "Fragment.hpp"
#include "Rasterizer.hpp"
#include "Misc.hpp"

#define MAX_VERTEX_NUM 6

template <class Attribute, class Uniform, class Varying>
class Pipeline {
    typedef Rasterizer<Varying> _Rasterizer;
    typedef Vertex<Varying> _Vertex;
    typedef Fragment<Varying> _Fragment;
    typedef BufferCursor<Attribute> _BufferCursor;
    
    typedef _Vertex** Primitive;
    
    coord_t width, height;
    WindowContext *context;
    
    // Shader processing types
    PolygonType vpType, gpType, fpType;
    
    // The number of vertexes to be drawed
    size_t drawCount;
    
    // Vertexes and fragments buffers
    Buffer &buffer = Buffer::sharedInstance();
    
    // Cursor for visiting attribute buffer and index buffer
    _BufferCursor cursor = _BufferCursor(attributeBuffer, indexBuffer);
    
    // Primitive pointer to be passed between processing functions
    Primitive primitive;
    size_t primitiveVertexNumber;
    
    _Rasterizer *rasterizer;
    
    bool enabledGeometryShader = false;
    
    GLfloat *zBuffer;
    
    // Common operations for `drawArrays` and `drawElements`
    ALWAYS_INLINE void _draw(PolygonType type, size_t begin, size_t count) {
        this->vpType = type;
        cursor.set(begin);
        drawCount = count;
        buffer.setVertexBufferSize(drawCount * sizeof(_Vertex));
        buffer.setPrimitiveVPBufferSize(drawCount * sizeof(_Vertex**));
        buffer.setPrimitiveFPBufferSize((drawCount * 3) * sizeof(_Vertex**)); // Splited primitives will take up more memory. Have not calculated its upper limit, just multiply three here.
        processVertexes();
    }
    
    // Primitive splitting functions in fragment processing
    ALWAYS_INLINE size_t _splitPrimitive(size_t vertexNumber, _Vertex **dstBufPtr) {
        size_t primitiveNumber = primitiveVertexNumber / vertexNumber;
        for (size_t i = 0; i < primitiveNumber; ++i) {
            for (size_t j = 0; j < vertexNumber; ++j) {
                *dstBufPtr = *primitive;
                ++dstBufPtr; ++primitive;
            }
        }
        return primitiveNumber;
    }
    ALWAYS_INLINE size_t _splitStripPrimitive(size_t vertexNumber, _Vertex **dstBufPtr) {
        size_t primitiveNumber = std::max((size_t)0, primitiveVertexNumber - vertexNumber + 1);
        for (size_t i = 0; i < primitiveNumber; ++i) {
            for (size_t j = 0; j < vertexNumber; ++j) {
                *dstBufPtr = *(primitive + j);
                ++dstBufPtr;
            }
            ++primitive;
        }
        return primitiveNumber;
    }
    ALWAYS_INLINE size_t _splitFanPrimitive(size_t vertexNumber, _Vertex **dstBufPtr) {
        size_t primitiveNumber = std::max((size_t)0, primitiveVertexNumber - 2);
        _Vertex *centroid = *primitive;
        for (size_t i = 0; i < primitiveNumber; ++i) {
            ++primitive;
            *dstBufPtr = centroid; ++dstBufPtr;
            *dstBufPtr = *primitive; ++dstBufPtr;
            *dstBufPtr = *(primitive + 1); ++dstBufPtr;
        }
        return primitiveNumber;
    }
    
    void processVertexes();
    void processGeometries();
    void processFragments();
    
public:
    std::vector<Attribute> attributeBuffer;
    std::vector<size_t> indexBuffer;
    Uniform uniform;
    
    std::function<void(const Attribute &, const Uniform &, _Vertex &)> vertexShader;
    std::function<void(const _Vertex * const, const Uniform &)> geometryShader;
    std::function<void(const _Fragment &, const Uniform &, vec4 &)> fragmentShader;
    
    Pipeline(WindowContext *context): context(context) {
        this->width = context->width; this->height = context->height;
        this->rasterizer = new _Rasterizer(width, height);
        this->zBuffer = context->zBuffer;
    }
    
    ~Pipeline() {
        delete rasterizer;
    }
    
    // External interfaces for drawing
    void drawArrays(PolygonType type, size_t begin, size_t count);
    void drawElements(PolygonType type, size_t begin, size_t count);
};

template <class Attribute, class Uniform, class Varying>
void Pipeline<Attribute, Uniform, Varying>::processVertexes() {
    // Execute vertex shader
    _Vertex *vertexBufPtr = (_Vertex*)buffer.getVertexBufferPtr();
    for (_Vertex *ptr = vertexBufPtr; ptr < vertexBufPtr + drawCount; ++ptr) {
        new (ptr)_Vertex;
        vertexShader(*cursor, uniform, *ptr);
        ++cursor;
    }
    
    // Assemble primitives for next shaders
    _Vertex **primitiveBufPtr = (_Vertex**)buffer.getPrimitiveVPBufferPtr();
    _Vertex **crtPrimitiveBufPtr = primitiveBufPtr;
    _Vertex *crtVertexBufPtr = vertexBufPtr;
    size_t primitiveNumber;
    size_t vertexNumber; // The number of vertexes of the primitive
    
    if (vpType == Points || vpType == Lines || vpType == LinesAdj || vpType == Triangles || vpType == TrianglesAdj) {
        // Vertexes which are not strips should be split into its elementary primitives
        switch (vpType) {
            case Points:
                vertexNumber = 1; break;
            case Lines:
                vertexNumber = 2; break;
            case LinesAdj:
                vertexNumber = 4; break;
            case Triangles:
                vertexNumber = 3; break;
            case TrianglesAdj:
                vertexNumber = 6; break;
                
            default:
                fatalError("Unknown polygon type"); // Should never reach here
        }
        
        primitiveNumber = drawCount / vertexNumber;
        for (size_t i = 0; i < primitiveNumber; ++i) {
            for (size_t j = 0; j < vertexNumber; ++j) {
                *crtPrimitiveBufPtr = crtVertexBufPtr;
                ++crtPrimitiveBufPtr; ++crtVertexBufPtr;
            }
        }
    } else {
        // Otherwise, pass them through to next shaders
        vertexNumber = drawCount;
        primitiveNumber = 1;
        for (size_t j = 0; j < vertexNumber; ++j) {
            *crtPrimitiveBufPtr = crtVertexBufPtr;
            ++crtPrimitiveBufPtr; ++crtVertexBufPtr;
        }
    }
    
    // Loop over the primitives
    crtPrimitiveBufPtr = primitiveBufPtr;
    for (size_t i = 0; i < primitiveNumber; ++i) {
        primitive = crtPrimitiveBufPtr;
        primitiveVertexNumber = vertexNumber;
        if (enabledGeometryShader) {
            // todo
        } else {
            // Geometry shader disabled, process fragments directly
            fpType = vpType;
            processFragments();
        }
        crtPrimitiveBufPtr += vertexNumber;
    }
}

template <class Attribute, class Uniform, class Varying>
void Pipeline<Attribute, Uniform, Varying>::processGeometries() {
    _Vertex *vertexBufferPtr = (_Vertex*)buffer.getVertexBufferPtr();
    
    if (enabledGeometryShader) {
        
    } else {
        // Geometry shader disabled, fragment shader input type is vertex shader output type
        fpType = vpType;
    }
}

template <class Attribute, class Uniform, class Varying>
void Pipeline<Attribute, Uniform, Varying>::processFragments() {
    // Screen mapping
    for (size_t i = 0; i < primitiveVertexNumber; ++i) {
        primitive[i]->screenMap(width, height);
    }
    
    // Adjacencies should be cutted out, cuz geometry shader will not output adjacencies and here the adjacencies are passed from vertex shader
    switch (fpType) {
        case LinesAdj:
        case LineStripAdj:
            ++primitive; // Ignore the first vertex pointer
            primitiveVertexNumber -= 2; // Discard the last vertex pointer
            break;
            
        case TrianglesAdj:
        case TriangleStripAdj:
            for (size_t i = 0; i <= primitiveVertexNumber / 2; ++i) {
                // Only store the vertex pointers with an even index
                primitive[i] = primitive[2 * i];
            }
            break;
            
        default:
            break;
    }
    
    // Split the primitive for rasterization
    PolygonType rasterizeType;
    size_t vertexNumber; // The number of rasterizing vertexes
    _Vertex **primitiveBufPtr = (_Vertex**)buffer.getPrimitiveFPBufferPtr();
    size_t primitiveNumber;
    switch (fpType) {
        case Points:
            rasterizeType = Points;
            vertexNumber = 1;
            primitiveNumber = _splitPrimitive(vertexNumber, primitiveBufPtr);
            break;
        case Lines:
        case LinesAdj:
            rasterizeType = Lines;
            vertexNumber = 2;
            primitiveNumber = _splitPrimitive(vertexNumber, primitiveBufPtr);
            break;
        case LineStrip:
        case LineStripAdj:
        case LineLoop:
            rasterizeType = Lines;
            vertexNumber = 2;
            primitiveNumber = _splitStripPrimitive(vertexNumber, primitiveBufPtr);
            break;
        case Triangles:
        case TrianglesAdj:
            rasterizeType = Triangles;
            vertexNumber = 3;
            primitiveNumber = _splitPrimitive(vertexNumber, primitiveBufPtr);
            break;
        case TriangleStrip:
        case TriangleStripAdj:
            rasterizeType = Triangles;
            vertexNumber = 3;
            primitiveNumber = _splitStripPrimitive(vertexNumber, primitiveBufPtr);
            break;
        case TriangleFan:
            rasterizeType = Triangles;
            vertexNumber = 3;
            primitiveNumber = _splitFanPrimitive(vertexNumber, primitiveBufPtr);
            break;
    }
    
    // For LineLoop drawing, append the last edge
    if (fpType == LineLoop && primitiveNumber >= 2) {
        *(primitiveBufPtr + 2 * primitiveNumber) = *(primitiveBufPtr + 2 * primitiveNumber - 1);
        *(primitiveBufPtr + 2 * primitiveNumber + 1) = *primitiveBufPtr;
        ++primitiveNumber;
    }
    
    // Rasterize
    _Vertex **crtPrimitiveBufPtr = primitiveBufPtr;
    for (size_t i = 0; i < primitiveNumber; ++i) {
        // Rasterize
        size_t fragNumber = rasterizer->rasterize(rasterizeType, crtPrimitiveBufPtr);
        crtPrimitiveBufPtr += vertexNumber;
        
        // Shade fragments
        _Fragment *frags = rasterizer->frags;
        _Fragment *fragEnd = frags + fragNumber;
        vec4 color;
        for (_Fragment *frag = frags; frag < fragEnd; ++frag) {
            coord_t pixelX = frag->pixelX, pixelY = frag->pixelY;
            // Clipping
            if (pixelX < 0 || pixelY < 0 || pixelX >= width || pixelY >= height) {
                continue;
            }
            
            // Z-test
            if (context->enabledZTest) {
                frag->normalizeZ();
                GLfloat *zBufPtr = zBuffer + pixelX * width + pixelY;
                if (frag->normalizedZ <= *zBufPtr) {
                    continue;
                } else {
                    *zBufPtr = frag->normalizedZ;
                }
            }
            
            fragmentShader(*frag, uniform, color);
            context->setPixel(pixelX, pixelY, color);
        }

    }
}

template <class Attribute, class Uniform, class Varying>
void Pipeline<Attribute, Uniform, Varying>::drawArrays(PolygonType type, size_t begin, size_t count) {
    cursor.cursorType = _BufferCursor::BufferCursorType::Array;
    _draw(type, begin, count);
}

template <class Attribute, class Uniform, class Varying>
void Pipeline<Attribute, Uniform, Varying>::drawElements(PolygonType type, size_t begin, size_t count) {
    cursor.cursorType = _BufferCursor::BufferCursorType::Element;
    _draw(type, begin, count);
}

#endif /* Pipeline_hpp */
