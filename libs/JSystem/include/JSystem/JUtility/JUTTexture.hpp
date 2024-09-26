#pragma once

#include <revolution.h>

struct ResTIMG {
    u8 mFormat;             // 0x0
    u8 mEnableAlpha;        // 0x1
    u16 mWidth;             // 0x2
    u16 mHeight;            // 0x4
    u8 mWrapS;              // 0x6
    u8 mWrapT;              // 0x7
    u8 mPaletteFormat;     // 0x8
    u8 _9;
    u16 mPaletteCount;      // 0xA
    u32 mPaletteOffs;       // 0xC
    u8 _10;
    u8 _11;
    u8 _12;
    u8 _13;
    u8 mMagType;            // 0x14
    u8 mMinType;            // 0x15
    u8 _16;
    u8 _17;
    u8 mTotalImgCount;      // 0x18
    u8 _19;
    u16 _1A;
    u32 mImgDataOffs;       // 0x1C
    u8* mImageData;         // 0x20
};

class JUTPalette {
public:
    void load();

    u8 _0[0xC];
    _GXTlut _C;
};

class JUTTexture {
public:
    inline JUTTexture() {

    }

    JUTTexture(int, int, _GXTexFmt);
    JUTTexture(const ResTIMG *, u8);
    ~JUTTexture();

    void storeTIMG(const ResTIMG *, u8);
    void storeTIMG(const ResTIMG *, JUTPalette *);
    void storeTIMG(const ResTIMG *, JUTPalette *, _GXTlut);
    void attachPalette(JUTPalette *);
    void init();
    void initTexObj();
    void initTexObj(_GXTlut);
    void load(_GXTexMapID);
    void capture(int, int, _GXTexFmt, bool, u8);
    void setEmbPaletteDelFlag(bool);

    GXTexObj mTexObj; // 0x0
    ResTIMG* _20;
    ResTIMG* _24;
    JUTPalette* _28;
    JUTPalette* mPalette; // 0x2C
    u8 mWrapS;              // 0x30
    u8 mWrapT;              // 0x31
    u8 mMagType;            // 0x32
    u8 mMinType;            // 0x33
    u16 _34;
    u16 _36;
    u16 _38;
    u8 _3A;
    u8 _3B;
    ResTIMG* _3C;
};