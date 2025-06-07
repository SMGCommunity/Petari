#pragma once

#include <revolution.h>

namespace nw4r {
    namespace ut {
        struct Rect {
        public:
            Rect() : left(0), top(0), right(0), bottom(0) {

            }

            Rect(f32 l, f32 t, f32 r, f32 b) : left(l), top(t), right(r), bottom(b) {
    
            }

            f32 left;
            f32 top;
            f32 right;
            f32 bottom;
        };
    };
};
