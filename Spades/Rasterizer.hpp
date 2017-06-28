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

enum drawType {
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
    
    inline size_t rasterize(drawType type, Vertex **vertexPtrs);
};

inline size_t Rasterizer::rasterize(drawType type, Vertex **vertexPtrs) {
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
    Vertex *leftVertex = vertexPtrs[0], *rightVertex = vertexPtrs[1];
    
    // Ensure the left vertex has the smaller x-coordinate
    coord_t leftPixelX = getPixelX(leftVertex), rightPixelX = getPixelY(rightVertex);
    if (leftPixelX > rightPixelX) {
        std::swap(leftVertex, rightVertex);
        std::swap(leftPixelX, rightPixelX);
    }
    
    coord_t leftPixelY = getPixelY(leftVertex), rightPixelY = getPixelY(rightVertex);
    
    
    size_t count = 0;
    return count;
}

size_t Rasterizer::rasterizeTriangle(Vertex **VertexPtrs) {
    fatalError("Unimplemented");
}

#endif /* Rasterizer_h */
