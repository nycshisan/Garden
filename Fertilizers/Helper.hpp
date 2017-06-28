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

#endif /* Helper_h */
