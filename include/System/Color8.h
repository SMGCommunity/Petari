#pragma once

#include "revolution/gx/GXStruct.h"

class Color8
{
public:
    Color8();
    Color8(u32);
    Color8(u8, u8, u8, u8);
    Color8(_GXColor);

    void set(_GXColor);
    void set(u8, u8, u8, u8);
    void operator=(const Color8 &);

    union
    {
        u32 fullColor;
        struct { u8 r, g, b, a; };
    };
};