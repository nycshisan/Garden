//
//  Rasterizer.hpp
//  Garden
//
//  Created by Nycshisan on 2017/5/3.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef Rasterizer_h
#define Rasterizer_h

#include <vector>
#include <memory>
#include <cmath>

#include "Vertex.hpp"
#include "Fragment.hpp"
#include "Misc.hpp"

enum DrawType {
    Points,
    Lines,
    LineStrip,
    LineLoop,
    Triangles,
    TriangleStrip,
    TriangleFan
};

template <class Varying>
class Rasterizer {
    unsigned int width, height;
    
    Fragment<Varying> *frags;
    
    size_t rasterizePoint(Vertex<Varying> **vertexPtrs);
    size_t rasterizeLine(Vertex<Varying> **vertexPtrs);
    size_t rasterizeTriangle(Vertex<Varying> **VertexPtrs);
    
    inline coord_t getPixelX(Vertex<Varying> *v) {
        return (coord_t)std::round(v->windowX * width);
    }
    inline coord_t getPixelY(Vertex<Varying> *v) {
        return (coord_t)std::round(v->windowY * height);
    }
    
public:
    Rasterizer(unsigned int width, unsigned int height, Fragment<Varying> *frags): width(width), height(height), frags(frags) {}
    
    inline size_t rasterize(DrawType type, Vertex<Varying> **vertexPtrs);
};

template <class Varying>
inline size_t Rasterizer<Varying>::rasterize(DrawType type, Vertex<Varying> **vertexPtrs) {
    switch (type) {
        case Points:
            return rasterizePoint(vertexPtrs);
        case Lines:
        case LineStrip:
        case LineLoop:
            return rasterizeLine(vertexPtrs);
        case Triangles:
        case TriangleStrip:
        case TriangleFan:
            return rasterizeTriangle(vertexPtrs);
            
        default:
            fatalError("Unknown draw type"); // Should never reach here
    }
}

template <class Varying>
size_t Rasterizer<Varying>::rasterizePoint(Vertex<Varying> **vertexPtrs) {
    size_t count = 0;
    Vertex<Varying> *vertex = vertexPtrs[0];
    int pointSize = vertex->pointSize;
    data_t z = vertex->position.z;
    
    // Simple offset to the top left point; need to be improved
    int radius = pointSize / 2;
    coord_t leftX = getPixelX(vertex) - radius;
    coord_t rightX = leftX + pointSize;
    coord_t topY = getPixelY(vertex) - radius;
    coord_t BottomY = topY + pointSize;
    
    Fragment<Varying> *crtFrag = frags;
    for (coord_t i = leftX; i < rightX; ++i) {
        for (coord_t j = topY; j < BottomY; ++j) {
            crtFrag->pixelX = i;
            crtFrag->pixelY = j;
            crtFrag->fragZ = z;
            crtFrag->varying = vertex->varying;
            
            ++count; ++crtFrag;
        }
    }
    return count;
}

template <class Varying>
size_t Rasterizer<Varying>::rasterizeLine(Vertex<Varying> **vertexPtrs) {
    // Setup
    Vertex<Varying> *&vertexA = vertexPtrs[0], *&vertexB = vertexPtrs[1];
    coord_t pixelXA = getPixelX(vertexA), pixelXB = getPixelX(vertexB), pixelYA = getPixelY(vertexA), pixelYB = getPixelY(vertexB);
    int dx = pixelXB - pixelXA, dy = pixelYB - pixelYA;
    int stepX = dx > 0 ? 1 : -1, stepY = dy > 0 ? 1 : -1;
    dx = abs(dx); dy = abs(dy); // For error calculating
    
    int error = 0;
    
    coord_t crtX = pixelXA, crtY = pixelYA;
    data_t crtZ = vertexA->position.z;
    Varying crtVarying = vertexA->varying;
    
    coord_t *indepCoordPtr, *depCoordPtr, stepEnd;
    int errorStep, errorThreshold, *indepStepPtr, *depStepPtr, stepLength;
    
    if (dx > dy) {
        // |k| < 1, step by x
        indepCoordPtr = &crtX;
        indepStepPtr = &stepX;
        depCoordPtr = &crtY;
        depStepPtr = &stepY;
        stepEnd = pixelXB + stepX;
        errorStep = 2 * dy;
        errorThreshold = 2 * dx;
        stepLength = dx;
    } else {
        // |k| >= 1, step by y
        indepCoordPtr = &crtY;
        indepStepPtr = &stepY;
        depCoordPtr = &crtX;
        depStepPtr = &stepX;
        stepEnd = pixelYB + stepY;
        errorStep = 2 * dx;
        errorThreshold = 2 * dy;
        stepLength = dy;
    }
    
    data_t stepZ = (vertexB->position.z - vertexA->position.z) / stepLength;
    Varying stepVarying = vertexB->varying;
    stepVarying.subtract(vertexA->varying);
    stepVarying.multiply(1.0 / stepLength);
    
    size_t count = 0;
    Fragment<Varying> *crtFrag = frags;
    
    // Bresenham algorithm
    do {
        crtFrag->pixelX = crtX;
        crtFrag->pixelY = crtY;
        crtFrag->fragZ = crtZ;
        crtFrag->varying = crtVarying;
        
        ++crtFrag; ++count;
        
        *indepCoordPtr += *indepStepPtr;
        error += errorStep;
        if (error >= errorThreshold) {
            *depCoordPtr += *depStepPtr;
            error -= errorThreshold;
        }
        
        crtZ += stepZ;
        crtVarying.add(stepVarying);
    } while (*indepCoordPtr != stepEnd);
    
    return count;
}

template <class Varying>
size_t Rasterizer<Varying>::rasterizeTriangle(Vertex<Varying> **VertexPtrs) {
    fatalError("Unimplemented");
}

#endif /* Rasterizer_h */
