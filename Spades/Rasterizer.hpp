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
    typedef Vertex<Varying> _Vertex;
    typedef Fragment<Varying> _Fragment;
    
    coord_t width, height;
    
    _Fragment *frags;
    
    size_t rasterizePoint(_Vertex **vertexPtrs);
    size_t rasterizeLine(_Vertex **vertexPtrs);
    size_t rasterizeTriangle(_Vertex **vertexPtrs);
    
    inline coord_t getPixelX(_Vertex *v) {
        return (coord_t)std::round(v->windowX * width);
    }
    inline coord_t getPixelY(_Vertex *v) {
        return (coord_t)std::round(v->windowY * height);
    }
    
public:
    Rasterizer(unsigned int width, unsigned int height, _Fragment *frags): width(width), height(height), frags(frags) {}
    
    inline size_t rasterize(DrawType type, _Vertex **vertexPtrs);
};

template <class Varying>
inline size_t Rasterizer<Varying>::rasterize(DrawType type, _Vertex **vertexPtrs) {
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
size_t Rasterizer<Varying>::rasterizePoint(_Vertex **vertexPtrs) {
    size_t count = 0;
    _Vertex *vertex = vertexPtrs[0];
    int pointSize = vertex->pointSize;
    data_t z = vertex->position.z;
    
    // Simple offset to the top left point; need to be improved
    int radius = pointSize / 2;
    coord_t leftX = getPixelX(vertex) - radius;
    coord_t rightX = leftX + pointSize;
    coord_t topY = getPixelY(vertex) - radius;
    coord_t BottomY = topY + pointSize;
    
    _Fragment *crtFrag = frags;
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
size_t Rasterizer<Varying>::rasterizeLine(_Vertex **vertexPtrs) {
    // Setup
    _Vertex *&vertexA = vertexPtrs[0], *&vertexB = vertexPtrs[1];
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
    _Fragment *crtFrag = frags;
    
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
size_t Rasterizer<Varying>::rasterizeTriangle(_Vertex **vertexPtrs) {
    _Vertex *vertexA = vertexPtrs[0], *vertexB = vertexPtrs[1], *vertexC = vertexPtrs[2];
    // Get bounding box
    coord_t xA = vertexA->windowX, xB = vertexB->windowX, xC = vertexC->windowX;
    coord_t yA = vertexA->windowY, yB = vertexB->windowY, yC = vertexC->windowY;
    coord_t xLeft = std::min(std::min(xA, xB), xC), xRight = std::max(std::max(xA, xB), xC);
    coord_t yTop = std::min(std::min(yA, yB), yC), yBottom = std::max(std::max(yA, yB), yC);
    // Edge check
    xLeft = std::max(xLeft, 0);
    xRight = std::min(xRight, height - 1);
    yTop = std::max(yTop, 0);
    yBottom = std::min(yBottom, width - 1);
    
    // Calculate proportion matrix
    vec4 &posA = vertexA->position, &posB = vertexB->position, &posC = vertexC->position;
    mat3 pMat = {{{posA.x, posB.x, posC.x},
                  {posA.y, posB.y, posC.y},
                  {posA.w, posB.w, posC.w}}};
    invert(pMat);
    
    // Mainloop
    size_t count = 0;
    vec3 pVec, windowPosVec;
    _Fragment *crtFrag = frags;
    data_t pSum, pA, pB, pC;
    
    for (coord_t crtX = xLeft; crtX <= xRight; ++crtX) {
        for (coord_t crtY = yTop; crtY <= yBottom; ++crtY) {
            windowPosVec = {(data_t)crtX, (data_t)crtY, 1.0};
            pVec = pMat * windowPosVec;
            pSum = pVec.x + pVec.y + pVec.z;
            pVec /= pSum;
            pA = pVec.x, pB = pVec.y, pC = pVec.z;
            
            if (pA < 0 || pB < 0 || pC < 0) {
                continue;
            }
            
            crtFrag->pixelX = crtX; crtFrag->pixelY = crtY;
            crtFrag->fragZ = pA * posA.z + pB * posB.z + pC * posC.z;
            Varying vA = vertexA->varying; vA.multiply(pA);
            Varying vB = vertexB->varying; vB.multiply(pB);
            Varying vC = vertexC->varying; vC.multiply(pC);
            vA.add(vB); vA.add(vC);
            
            crtFrag->varying = vA;
            
            ++count; ++crtFrag;
        }
    }
    
    return count;
}

#endif /* Rasterizer_h */
