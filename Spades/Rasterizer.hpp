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

#include "Fragment.hpp"

enum DrawType {
    Points,
    Lines,
    LineStrip,
    LineLoop,
    Triangles,
    TriangleStrip,
    TriangleFan
};

class Rasterizer {
    unsigned int width, height;
    
    Fragment *frags;
    
    size_t rasterizePoint(Vertex **vertexPtrs);
    size_t rasterizeLine(Vertex **vertexPtrs);
    size_t rasterizeTriangle(Vertex **VertexPtrs);
    
    inline coord_t getPixelX(Vertex *v) {
        return (coord_t)std::round(v->windowX * width);
    }
    inline coord_t getPixelY(Vertex *v) {
        return (coord_t)std::round(v->windowY * height);
    }
    
public:
    Rasterizer(unsigned int width, unsigned int height, Fragment *frags): width(width), height(height), frags(frags) {}
    
    inline size_t rasterize(DrawType type, Vertex **vertexPtrs);
};

inline size_t Rasterizer::rasterize(DrawType type, Vertex **vertexPtrs) {
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

size_t Rasterizer::rasterizePoint(Vertex **vertexPtrs) {
    size_t count = 0;
    Vertex *vertex = vertexPtrs[0];
    int pointSize = vertex->pointSize;
    
    // Simple offset to the top left point; need to be improved
    int radius = pointSize / 2;
    coord_t leftX = getPixelX(vertex) - radius;
    coord_t rightX = leftX + pointSize;
    coord_t topY = getPixelY(vertex) - radius;
    coord_t BottomY = topY + pointSize;
    
    Fragment *crtFrag = frags;
    for (coord_t i = leftX; i < rightX; ++i) {
        for (coord_t j = topY; j < BottomY; ++j) {
            crtFrag->pixelX = i;
            crtFrag->pixelY = j;
            ++count; ++crtFrag;
        }
    }
    return count;
}

size_t Rasterizer::rasterizeLine(Vertex **vertexPtrs) {
    // Setup
    coord_t pixelXA = getPixelX(vertexPtrs[0]), pixelXB = getPixelX(vertexPtrs[1]), pixelYA = getPixelY(vertexPtrs[0]), pixelYB = getPixelY(vertexPtrs[1]);
    int dx = pixelXB - pixelXA, dy = pixelYB - pixelYA;
    int stepX = dx > 0 ? 1 : -1, stepY = dy > 0 ? 1 : -1;
    dx = abs(dx); dy = abs(dy);
    
    int error = 0;
    
    coord_t crtX = pixelXA, crtY = pixelYA;
    
    coord_t *indepCoordPtr, *depCoordPtr, stepEnd;
    int errorStep, errorThreshold, *indepStepPtr, *depStepPtr;
    
    if (dx > dy) {
        // |k| < 1, step by x
        indepCoordPtr = &crtX;
        indepStepPtr = &stepX;
        depCoordPtr = &crtY;
        depStepPtr = &stepY;
        stepEnd = pixelXB + stepX;
        errorStep = 2 * dy;
        errorThreshold = 2 * dx;
    } else {
        // |k| >= 1, step by y
        indepCoordPtr = &crtY;
        indepStepPtr = &stepY;
        depCoordPtr = &crtX;
        depStepPtr = &stepX;
        stepEnd = pixelYB + stepY;
        errorStep = 2 * dx;
        errorThreshold = 2 * dy;
    }
    
    size_t count = 0;
    Fragment *crtFrag = frags;
    
    // Bresenham algorithm
    do {
        crtFrag->pixelX = crtX;
        crtFrag->pixelY = crtY;
        
        ++crtFrag; ++count;
        
        *indepCoordPtr += *indepStepPtr;
        error += errorStep;
        if (error >= errorThreshold) {
            *depCoordPtr += *depStepPtr;
            error -= errorThreshold;
        }
    } while (*indepCoordPtr != stepEnd);
    
    return count;
}

size_t Rasterizer::rasterizeTriangle(Vertex **VertexPtrs) {
    fatalError("Unimplemented");
}

#endif /* Rasterizer_h */
