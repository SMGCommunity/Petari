#include "System/Color8.h"

Color8::Color8()
    : fullColor(-1)
    { }

Color8::Color8(u8 inR, u8 inG, u8 inB, u8 inA)
    : r(inR), g(inG), b(inB), a(inA)
    { }

/* todo -- have it use the stack properly?? */
Color8::Color8(_GXColor color)
{
    _GXColor temp;
    temp.r = color.r;
    temp.g = color.g;
    temp.b = color.b;
    temp.a = color.a;
    r = temp.r;
    g = temp.g;
    b = temp.b;
    a = temp.a;
}

void Color8::set(_GXColor color)
{
    u8 temp_r = color.r;
    u8 temp_g = color.g;
    u8 temp_b = color.b;
    u8 temp_a = color.a;
    r = temp_r;
    g = temp_g;
    b = temp_b;
    a = temp_a;
}

void Color8::set(u8 inR, u8 inG, u8 inB, u8 inA)
{
    r = inR;
    g = inG;
    b = inB;
    a = inA;
}