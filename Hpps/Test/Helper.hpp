//
//  Helper.hpp
//  Garden
//
//  Created by Nycshisan on 2017/8/26.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef Helper_h
#define Helper_h

#define ATTRIBUTE CONCAT(Attribute_, TEST_NAME)
#define UNIFORM class CONCAT(Uniform_, TEST_NAME)
#define VARYING CONCAT(Varying_, TEST_NAME)
#define VS CONCAT(vertexShader_, TEST_NAME)
#define GS CONCAT(geometryShader_, TEST_NAME)
#define FS CONCAT(fragmentShader_, TEST_NAME)
#define DEF_ATTRIBUTE class ATTRIBUTE
#define DEF_UNIFORM UNIFORM
#define DEF_PIPELINE typedef Pipeline<ATTRIBUTE, CONCAT(Uniform_, TEST_NAME), VARYING> CONCAT(Pipeline_, TEST_NAME)
#define DEF_VS void VS(const ATTRIBUTE &attribute, const UNIFORM &uniform, Vertex<VARYING> &vertex)
#define DEF_FS void FS(const Fragment<VARYING> &frag, const UNIFORM &uniform, vec4 &fragColor)
#define DEF_PREPARE void prepare(CONCAT(Pipeline_, TEST_NAME) &pipeline)
#define DEF_DRAW void draw(CONCAT(Pipeline_, TEST_NAME) &pipeline)

#endif /* Helper_h */
