#pragma once

#include <revolution.h>

struct ResTIMG {
    u8 mFormat;             // _0
    u8 mEnableAlpha;        // _1
    u16 mWidth;             // _2
    u16 mHeight;            // _4
    u8 mWrapS;              // _6
    u8 mWrapT;              // _7
    u8 mPaletteFormat;     // _8
    u8 _9;
    u16 mPaletteCount;      // _A
    u32 mPaletteOffs;       // _C
    u8 _10;
    u8 _11;
    u8 _12;
    u8 _13;
    u8 mMagType;            // _14
    u8 mMinType;            // _15
    u8 _16;
    u8 _17;
    u8 mTotalImgCount;      // _18
    u8 _19;
    u16 _1A;
    u32 mImgDataOffs;       // _1C
    u8* mImageData;         // _20
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

    GXTexObj mTexObj; // _0
    ResTIMG* _20;
    ResTIMG* _24;
    JUTPalette* _28;
    JUTPalette* mPalette; // _2C
    u8 mWrapS;              // _30
    u8 mWrapT;              // _31
    u8 mMagType;            // _32
    u8 mMinType;            // _33
    u16 _34;
    u16 _36;
    u16 _38;
    u8 _3A;
    u8 _3B;
    ResTIMG* _3C;
};