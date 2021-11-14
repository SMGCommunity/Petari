#pragma once

#include "revolution.h"

class J3DAlphaComp {
public:
    J3DAlphaComp();

    u16 _0;
    u8 _2;
    u8 _3;

    static u16 sUnk;
};

class J3DCurrentMtx {
public:
    void setCurrentTexMtx(u8, u8, u8, u8, u8, u8, u8, u8);

    u32 _0;
    u32 _4;
};