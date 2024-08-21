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

            f32 mLeft;      // 0x0
            f32 mTop;       // 0x4
            f32 mRight;     // 0x8
            f32 mBottom;    // 0xC
        };
    };
};