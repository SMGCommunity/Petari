#pragma once

#include <revolution.h>

class Color8 {
public:
    Color8() {
        *this = 0xffffffff;
    }

    Color8(u32 color) {
        *this = color;
    }

    Color8(GXColor color) {
        mGXColor = color;
    }

    Color8(u8 _r, u8 _g, u8 _b, u8 _a) NO_INLINE {
        mGXColor.r = _r;
        mGXColor.g = _g;
        mGXColor.b = _b;
        mGXColor.a = _a;
    }

    inline operator GXColor() const {
        return mGXColor;
    }

    Color8& operator =(u32 color) {
        *reinterpret_cast<u32*>(this) = color;
        return *this;
    }

    Color8& operator =(const GXColor& color) {
        mGXColor = color;
        return *this;
    }

    const Color8& operator=(const Color8 &other) {
        mGXColor = other.mGXColor;
        return *this;
    }

    void set(u8 _r, u8 _g, u8 _b, u8 _a);

    GXColor mGXColor;
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
