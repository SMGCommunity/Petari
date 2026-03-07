#pragma once

#include <revolution.h>

enum JUTTransparency { UNK0, UNK1 };

struct ResTLUT {
    /* 0x0 */ u8 mFormat;
    /* 0x1 */ u8 mTransparency;
    /* 0x2 */ u16 mLutNum;
};

class JUTPalette {
public:
    JUTPalette(_GXTlut p1, _GXTlutFmt p2, JUTTransparency p3, u16 p4, void* p5) { this->storeTLUT(p1, p2, p3, p4, p5); }

    JUTPalette(GXTlut tlutNo, ResTLUT* p_tlutRes) { storeTLUT(tlutNo, p_tlutRes); }

    void storeTLUT(GXTlut, ResTLUT*);
    void storeTLUT(GXTlut, GXTlutFmt, JUTTransparency, u16, void*);
    bool load();

    /* 0x00 */ GXTlutObj mObj;
    /* 0x0C */ u8 mName;
    /* 0x0D */ u8 mFormat;
    /* 0x10 */ ResTLUT* mColorTable;
    /* 0x14 */ u16 mLutNum;
    /* 0x16 */ u8 mTransparency;
};
