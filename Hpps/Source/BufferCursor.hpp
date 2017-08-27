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
        Array,
        Element
    };
    
    BufferCursorType cursorType;
    
    std::vector<Attribute> &attributeBuffer;
    std::vector<size_t> &indexBuffer;
    
    size_t _index;
    
    BufferCursor(std::vector<Attribute> &attributeBuffer, std::vector<size_t> &indexBuffer): attributeBuffer(attributeBuffer), indexBuffer(indexBuffer) {}
    
    ALWAYS_INLINE Attribute& operator * () {
        if (cursorType == Array) {
            // drawVertexes mode
            return attributeBuffer[_index];
        } else {
            // drawElements mode
            return attributeBuffer[indexBuffer[_index]];
        }
    }
    
    ALWAYS_INLINE void operator ++ () {
        ++_index;
    }
    
    ALWAYS_INLINE void begin() {
        _index = 0;
    }
    
    ALWAYS_INLINE void set(size_t index) {
        _index = index;
    }
    
    ALWAYS_INLINE bool end() {
        if (cursorType == Array) {
            // drawArrays mode
            return _index == attributeBuffer.size();
        } else {
            // drawElements mode
            return _index == indexBuffer.size();
        }
    }
    
    ALWAYS_INLINE size_t size() {
        if (cursorType == Array) {
            // drawArrays mode
            return attributeBuffer.size();
        } else {
            // drawElements mode
            return indexBuffer.size();
        }
    }
};

#endif /* BufferCursor_h */
