//
//  Macro.hpp
//  Garden
//
//  Created by Nycshisan on 2017/6/25.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef Helper_h
#define Helper_h

// C++ standards restrain the unwinding of macro arguments used with `##`
#define CONCAT_LITERAL(L, R) L##R
#define CONCAT(L, R) CONCAT_LITERAL(L, R)

#define fatalError(MSG) do {\
    printf(MSG);\
    assert(0);\
} while(0)

// Common definitions for seeds
#define VS void vertexShader(const Attribute &a, Vertex &v)
#define FS void fragmentShader(const Fragment &f, const Uniform &u, vec4 &color)
#define PREPARE void prepare(Pipeline<Attribute, Uniform, Varying> &pipeline)
#define DRAW void draw(Pipeline<Attribute, Uniform, Varying> &pipeline)

#endif /* Helper_h */
