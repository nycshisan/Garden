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
    Point,
    Line,
    LineStrip,
    LineLoop,
    Triangle,
    TriangleStrip,
    TriangleFan
};

class Rasterizer {
    unsigned int width, height;
    
    Fragment *frags;
    
    size_t rasterizePoint(Vertex **vertexPtrs);
    size_t rasterizeLine(Vertex **vertexPtrs);
    size_t rasterizeTriangle(Vertex **VertexPtrs);
    
public:
    Rasterizer(unsigned int width, unsigned int height, Fragment *frags): width(width), height(height), frags(frags) {}
    
//    size_t rasterize(Vertex *vertexes) {
//        size_t count = 0;
//        size_t min_i = vertexes[0].position.x * width;
//        size_t max_i = vertexes[2].position.x * width;
//        size_t min_j = vertexes[0].position.y * height;
//        size_t max_j = vertexes[2].position.y * height;
//        
//        Fragment *crt_frag = frags;
//        for (size_t i = min_i; i < max_i; ++i) {
//            for (size_t j = min_j; j < max_j; ++j) {
//                crt_frag->x = i;
//                crt_frag->y = j;
//                ++count; ++crt_frag;
//            }
//        }
//        
//        return count;
//    }
    
    inline size_t rasterize(drawType type, Vertex **vertexPtrs);
};

inline size_t Rasterizer::rasterize(drawType type, Vertex **vertexPtrs) {
    switch (type) {
        case Point:
            return rasterizePoint(vertexPtrs);
        case Line:
        case LineStrip:
        case LineLoop:
            return rasterizeLine(vertexPtrs);
        case Triangle:
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
    vertex->convertToWindowCoord();
    int pointSize = vertex->pointSize;
    
    // Simple offset to the top left point; need to be improved
    int radius = pointSize / 2;
    size_t leftX = (size_t)std::round(vertex->windowX * width) - radius;
    size_t rightX = leftX + pointSize;
    size_t topY = (size_t)std::round(vertex->windowY * height) - radius;
    size_t BottomY = topY + pointSize;
    
    Fragment *crtFrag = frags;
    for (size_t i = leftX; i < rightX; ++i) {
        for (size_t j = topY; j < BottomY; ++j) {
            crtFrag->pixelX = i;
            crtFrag->pixelY = j;
            ++count; ++crtFrag;
        }
    }
    return count;
}

size_t Rasterizer::rasterizeLine(Vertex **vertexPtrs) {
    return 0;
}

size_t Rasterizer::rasterizeTriangle(Vertex **VertexPtrs) {
    fatalError("Unimplemented");
}

#endif /* Rasterizer_h */
