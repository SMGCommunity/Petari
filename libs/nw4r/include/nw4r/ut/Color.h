#pragma once

#include <revolution/gx/GXStruct.h>

namespace nw4r {
    namespace ut {
        struct Color : public GXColor {
            static const u32 WHITE = 0xFFFFFFFF;

            Color() {
                operator=(WHITE);
            }

            Color(u32 color) {
                operator=(color);
            }

            Color(int red, int green, int blue, int alpha) {

            }

            Color& operator=(u32 color) {
                ToU32ref() = color;
                return *this;
            }

            u32& ToU32ref() {
                return *reinterpret_cast<u32*>(this);
            }
        };
    };
};