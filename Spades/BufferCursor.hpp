//
//  BufferCursor.hpp
//  Garden
//
//  Created by Nycshisan on 2017/7/15.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef BufferCursor_h
#define BufferCursor_h

#include <vector>

#include "Misc.hpp"

template <class Attribute>
class BufferCursor {
    template <class _Attribute, class Uniform, class Varying>
    friend class Pipeline;
    
    enum BufferCursorType {
        Direct,
        Element
    };
    
    BufferCursorType type;
    
    std::vector<Attribute> &vertexBuffer;
    std::vector<size_t> &indexBuffer;
    
    size_t index;
    
    BufferCursor(std::vector<Attribute> &vertexBuffer, std::vector<size_t> &indexBuffer): vertexBuffer(vertexBuffer), indexBuffer(indexBuffer) {}
    
    ALWAYS_INLINE Attribute& operator * () {
        if (type == Direct) {
            // draw mode
            return vertexBuffer[index];
        } else {
            // drawElements mode
            return vertexBuffer[indexBuffer[index]];
        }
    }
    
    ALWAYS_INLINE void operator ++ () {
        ++index;
    }
    
    ALWAYS_INLINE void begin() {
        index = 0;
    }
    
    ALWAYS_INLINE bool end() {
        if (type == Direct) {
            // draw mode
            return index == vertexBuffer.size();
        } else {
            // drawElements mode
            return index == indexBuffer.size();
        }
    }
    
    ALWAYS_INLINE size_t size() {
        if (type == Direct) {
            // draw mode
            return vertexBuffer.size();
        } else {
            // drawElements mode
            return indexBuffer.size();
        }
    }
};

#endif /* BufferCursor_h */
