#pragma once

#include <revolution.h>

namespace nw4r {
    namespace ut {
        struct Rect {
        public:
            Rect() : left(0), top(0), right(0), bottom(0) {}

            Rect(f32 l, f32 t, f32 r, f32 b) : left(l), top(t), right(r), bottom(b) {}

            f32 GetWidth() const { return right - left; }
            f32 GetHeight() const { return bottom - top; }

            void MoveTo(f32 x, f32 y) {
                right = x + GetWidth();
                left = x;
                bottom = y + GetHeight();
                top = y;
            }

            f32 left;
            f32 top;
            f32 right;
            f32 bottom;
        };
    };  // namespace ut
};  // namespace nw4r
