#pragma once

#include <revolution.h>

class Color8 {
public:
    Color8(_GXColor);

    Color8(u8 _r, u8 _g, u8 _b, u8 _a) {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }

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
        struct {
            u8 r;
            u8 g;
            u8 b;
            u8 a;
        };
        u32 mColor;
    };
};

class Color10 {
public:
    Color10() : r(0x3FF), g(0x3FF), b(0x3FF), a(0x3FF) {
        
    }

    u16 r;
    u16 g;
    u16 b;
    u16 a;
};