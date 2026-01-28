#pragma once

#include "JSystem/JUtility/TColor.hpp"
#include <cstdio>
#include <revolution.h>

class JUTDirectPrint {
public:
    static JUTDirectPrint* start();

    void erase(int, int, int, int);
    void drawChar(int, int, int);
    void changeFrameBuffer(void*, u16, u16);
    /* -------- */ void print(u16, u16, char const*, ...);
    void printSub(u16, u16, char const*, va_list, bool);
    void drawString(u16, u16, char*);
    void drawString_f(u16, u16, char const*, ...);
    void setCharColor(u8, u8, u8);
    void setCharColor(JUtility::TColor);

    static JUTDirectPrint* getManager() { return sDirectPrint; }
    JUtility::TColor getCharColor() const { return mCharColor; }

    void* field_0x00;
    u16 mFrameBufferWidth;
    u16 mFrameBufferHeight;
    u16 mStride;
    u16 field_0x0A;
    size_t mFrameBufferSize;
    u8 field_0x10[4];
    u16* mFrameBuffer;
    JUtility::TColor mCharColor;
    u16 mCharColor_Y;
    u16 mCharColor_Cb;
    u16 mCharColor_Cb2;
    u16 mCharColor_Cb4;
    u16 mCharColor_Cr;
    u16 mCharColor_Cr2;
    u16 mCharColor_Cr4;
    u16 field_0x2A;

    static JUTDirectPrint* sDirectPrint;
};
