#pragma once

#include <revolution.h>

class Color8 {
public:
    Color8(_GXColor);
    Color8(u8, u8, u8, u8);
    Color8(u32 color) {
        mColor = color;
    }

    inline operator GXColor() const {
        GXColor color;
        color.r = r;
        color.g = g;
        color.b = b;
        color.a = a;
        return color;
    }

    union {
        u8 r, g, b, a;
        u32 mColor;
    };
};