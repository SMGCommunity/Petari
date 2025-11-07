#pragma once

#include <revolution.h>

enum JUTTransparency {
    
};

struct ResTLUT {
    /* 0x0 */ GXTlutFmt mFormat;
    /* 0x1 */ u8 mTransparency;
    /* 0x2 */ u16 mLutNum;
};

class JUTPalette {
public:
    void storeTLUT(GXTlut, ResTLUT*);
    void storeTLUT(GXTlut, GXTlutFmt, JUTTransparency, u16, void*);
    void load();

    /* 0x00 */ GXTlutObj mObj;
    /* 0x0C */ u8 mName;
    /* 0x0D */ u8 mFormat;
    /* 0x10 */ void* mLut;
    /* 0x14 */ u16 mLutNum;
    /* 0x16 */ u8 mTransparency;
};
