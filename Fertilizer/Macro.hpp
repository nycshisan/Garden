//
//  Macro.hpp
//  Garden
//
//  Created by Nycshisan on 2017/6/25.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef Macro_h
#define Macro_h

// C++ standards restrain the unwinding of macro arguments used with `##`
#define CONCAT_LITERAL(L, R) L##R
#define CONCAT(L, R) CONCAT_LITERAL(L, R)

#define CrtAttribute CONCAT(Attribute_, CRT_TASK)
#define CrtUniform CONCAT(Uniform_, CRT_TASK)
#define CrtVarying CONCAT(Varying_, CRT_TASK)

#endif /* Macro_h */
