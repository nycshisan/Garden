//
//  SeedsHelper.hpp
//  Garden
//
//  Created by Nycshisan on 2017/6/25.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef SeedsHelper_h
#define SeedsHelper_h

// Common definitions for seeds
#define VERTEX_SHADER void vertexShader(const Attribute &a, Vertex<Varying> &v)
#define FRAGMENT_SHADER void fragmentShader(const Fragment<Varying> &f, const Uniform &u, vec4 &color)
#define PREPARE void prepare(Pipeline<Attribute, Uniform, Varying> &pipeline)
#define DRAW void draw(Pipeline<Attribute, Uniform, Varying> &pipeline)

#endif /* SeedsHelper_h */
