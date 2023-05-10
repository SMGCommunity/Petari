#pragma once

#include <revolution.h>

namespace nw4r {
    namespace ut {
        class Rect {
        public:

            inline Rect(f32 left, f32 top, f32 right, f32 bottom) {
                mLeft = left;
                mTop = top;
                mRight = right;
                mBottom = bottom;
            }

            f32 mLeft;      // _0
            f32 mTop;       // _4
            f32 mRight;     // _8
            f32 mBottom;    // _C
        };
    };
};