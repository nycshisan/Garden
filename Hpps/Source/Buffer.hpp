//
//  Buffer.hpp
//  Garden
//
//  Created by Nycshisan on 2017/8/25.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef Buffer_h
#define Buffer_h

#include <stdlib.h>
#include <cassert>

#include "Misc.hpp"

#define DEF_BUF_VARIABLES(BUF_NAME) \
void *CONCAT(BUF_NAME, BufferBeginPtr) = nullptr; \
void *CONCAT(BUF_NAME, BufferEndPtr) = nullptr; \
size_t CONCAT(BUF_NAME, BufferSize) = 0;

#define DEF_BUF_METHODS(BUF_NAME, BUF_NAME_CAPITAL) \
ALWAYS_INLINE void *CONCAT_TRIPLE(get, BUF_NAME_CAPITAL, BufferBeginPtr)() { \
    assert(CONCAT(BUF_NAME, BufferBeginPtr) != nullptr); \
    return CONCAT(BUF_NAME, BufferBeginPtr); \
} \
ALWAYS_INLINE void *CONCAT_TRIPLE(get, BUF_NAME_CAPITAL, BufferEndPtr)() { \
    assert(CONCAT(BUF_NAME, BufferEndPtr) != nullptr); \
    return CONCAT(BUF_NAME, BufferEndPtr); \
} \
ALWAYS_INLINE void CONCAT_TRIPLE(set, BUF_NAME_CAPITAL, BufferSize)(size_t size) { \
    if (size > CONCAT(BUF_NAME, BufferSize)) { \
        CONCAT(BUF_NAME, BufferSize) = size; \
        free(CONCAT(BUF_NAME, BufferBeginPtr)); \
        CONCAT(BUF_NAME, BufferBeginPtr) = malloc(size); \
        CONCAT(BUF_NAME, BufferEndPtr) = (char*)CONCAT(BUF_NAME, BufferBeginPtr) + size; \
    } \
}

class Buffer {
    DEF_BUF_VARIABLES(vertex)
    
    DEF_BUF_VARIABLES(primitiveVP)
    DEF_BUF_VARIABLES(primitiveFP)
    
    DEF_BUF_VARIABLES(fragment)
    
    Buffer() {}
    Buffer(const Buffer &buffer) = delete;
    Buffer(Buffer &&buffer) = delete;
    
public:
    static Buffer &sharedInstance() {
        static Buffer instance;
        return instance;
    }
    
    DEF_BUF_METHODS(vertex, Vertex)
    
    DEF_BUF_METHODS(primitiveVP, PrimitiveVP)
    DEF_BUF_METHODS(primitiveFP, PrimitiveFP)
    
    DEF_BUF_METHODS(fragment, Fragment)
    
    ~Buffer() {
        free(vertexBufferBeginPtr);
        free(primitiveVPBufferBeginPtr);
        free(primitiveFPBufferBeginPtr);
        free(fragmentBufferBeginPtr);
    }
};

#undef DEF_BUF_VARIABLES
#undef DEF_BUF_METHODS

#endif /* Buffer_h */
