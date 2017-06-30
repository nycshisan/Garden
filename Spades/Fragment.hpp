//
//  Fragment.hpp
//  Garden
//
//  Created by Nycshisan on 2017/5/3.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef Fragment_h
#define Fragment_h

#include "Vec.hpp"

template <class Varying>
class Fragment {
    
public:
    coord_t pixelX, pixelY;
    data_t fragZ;
    
    Varying varying;
};

#endif /* Fragment_h */
