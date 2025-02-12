#pragma once

#include <revolution.h>

namespace nw4r {
    namespace ut {
        struct Rect {
        public:

            Rect(f32 l, f32 t, f32 r, f32 b) : left(l), top(t), right(r), bottom(b) {
    
            }

            f32 left;
            f32 top;
            f32 right;
            f32 bottom;
        };
    };
};
