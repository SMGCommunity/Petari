#pragma once

#include "Game/NameObj/NameObj.hpp"

class ResTIMG;
class JUTTexture;
class FootPrintInfo;

class FootPrint : public NameObj {
public:
    FootPrint(const char *, s32, s32);
    void setTexture(ResTIMG *);

    JUTTexture *_C;
    FootPrintInfo *_10;
    f32 _14;
    f32 _18;
    f32 _1C;
    s32 _20;
    u32 _24;
    u32 _28;
    f32 _2C;
    f32 _30;
    f32 _34;
    f32 _38;
    u8 _3C;
};
