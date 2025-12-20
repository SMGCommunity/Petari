#pragma once

#include <revolution/gx/GXStruct.h>

namespace nw4r {
    namespace ut {
        struct Color : public GXColor {
        public:
            static const int ALPHA_MAX = 255;

            static const u32 WHITE = 0xFFFFFFFF;

            Color() { *this = 0xFFFFFFFF; }

            Color(u32 color) { *this = color; }

            Color(const GXColor& color) { *this = color; }

            Color& operator=(u32 color) {
                ToU32ref() = color;
                return *this;
            }

            Color& operator=(const GXColor& color) { return operator=(*reinterpret_cast< const u32* >(&color)); }

            ~Color() {}

            operator u32() const { return ToU32ref(); }

            u32& ToU32ref() { return *reinterpret_cast< u32* >(this); }
            const u32& ToU32ref() const { return *reinterpret_cast< const u32* >(this); }
        };
    };  // namespace ut
};  // namespace nw4r
