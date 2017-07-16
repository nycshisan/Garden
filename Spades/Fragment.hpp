//
//  Fragment.hpp
//  Garden
//
//  Created by Nycshisan on 2017/5/3.
//  Copyright © 2017年 陈十三. All rights reserved.
//

#ifndef Fragment_h
#define Fragment_h

#include "Misc.hpp"
#include "Vec.hpp"
#include "Mat.hpp"

template <class Varying>
class Fragment {
    
public:
    coord_t pixelX, pixelY;
    data_t fragZ, normalizedZ;
    
    Varying varying;
    
    // This function is used to normalize the `z` value to [0, 1] for z-testing
    ALWAYS_INLINE void normalizeZ() {
        if (fragZ == 0) {
            normalizedZ = 0.5;
        } else {
            if (fragZ < 0) {
                normalizedZ = fragZ / -2.0;
            } else {
                normalizedZ = fragZ / 2.0 + 0.5;
            }
        }
    }
};

#endif /* Fragment_h */
