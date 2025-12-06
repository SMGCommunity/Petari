#pragma once

#include <revolution.h>

class Color8 {
public:
    Color8() {}

    Color8(GXColor color) { set(color); }

    Color8(u8 r, u8 g, u8 b, u8 a) { set(r, g, b, a); }

    Color8(u32 color) { mColor = color; }

    inline operator GXColor() const { return mGXColor; }

    inline operator u32() const { return mColor; }

    inline const Color8& operator=(const GXColor& other) {
        mGXColor = other;
        return *this;
    }

    inline const Color8& operator=(const Color8& other) {
        r = other.r;
        g = other.g;
        b = other.b;
        a = other.a;
        return *this;
    }

    void set(u8 _r, u8 _g, u8 _b, u8 _a) {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }

    void set(GXColor color) {
        r = color.r;
        g = color.g;
        b = color.b;
        a = color.a;
    }

    union {
        struct {
            u8 r;
            u8 g;
            u8 b;
            u8 a;
        };
        u32 mColor;
        GXColor mGXColor;
    };
};

class Color10 {
public:
    Color10() : r(0x3FF), g(0x3FF), b(0x3FF), a(0x3FF) {}

    u16 r;
    u16 g;
    u16 b;
    u16 a;
};
