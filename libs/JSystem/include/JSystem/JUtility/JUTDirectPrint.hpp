#pragma once

#include <revolution.h>
#include <cstdio>
#include "JSystem/JUtility/TColor.hpp"

class JUTDirectPrint {
public:
    static JUTDirectPrint* start();

    void changeFrameBuffer(void*, u16, u16);

    void*            field_0x00;
    u16              mFrameBufferWidth;
    u16              mFrameBufferHeight;
    u16              mStride;
    u16              field_0x0A;
    size_t           mFrameBufferSize;
    u8               field_0x10[4];
    u16*             mFrameBuffer;
    JUtility::TColor mCharColor;
    u16              mCharColor_Y;
    u16              mCharColor_Cb;
    u16              mCharColor_Cb2;
    u16              mCharColor_Cb4;
    u16              mCharColor_Cr;
    u16              mCharColor_Cr2;
    u16              mCharColor_Cr4;
    u16              field_0x2A;

    static JUTDirectPrint* sDirectPrint;
};
