#pragma once

#include <revolution.h>

class JUTPalette;

struct ResTIMG {
    /* 0x00 */ u8 mFormat;
    /* 0x01 */ u8 mTransparency;
    /* 0x02 */ u16 mWidth;
    /* 0x04 */ u16 mHeight;
    /* 0x06 */ u8 mWrapS;
    /* 0x07 */ u8 mWrapT;
    /* 0x08 */ u8 mPaletteName;
    /* 0x09 */ u8 mPaletteFormat;
    /* 0x0A */ u16 mPaletteNum;
    /* 0x0C */ u32 mPaletteDataOffset;
    /* 0x10 */ bool mMipmap;
    /* 0x11 */ bool mDoEdgeLod;
    /* 0x12 */ bool mBiasClamp;
    /* 0x13 */ u8 mMaxAnisotropy;
    /* 0x14 */ u8 mMinType;
    /* 0x15 */ u8 mMagType;
    /* 0x16 */ u8 mMinLod;
    /* 0x17 */ u8 mMaxLod;
    /* 0x18 */ u8 mImageNum;
    /* 0x19 */ u8 _19;
    /* 0x1A */ s16 mLodBias;
    /* 0x1C */ u32 mImageDataOffset;
};

class JUTTexture {
public:
    /// @brief Creates a new `JUTTexture`.
    JUTTexture() {
        setCaptureFlag(false);
        mEmbPalette = NULL;
        mTIMG = NULL;
    }

    /// @brief Creates a new `JUTTexture`.
    /// @param width The width of the texture, in pixels.
    /// @param height The height of the texture, in pixels.
    /// @param format The type of the texture.
    JUTTexture(int width, int height, GXTexFmt format);

    /// @brief Creates a new `JUTTexture`.
    /// @param pTIMG TODO.
    /// @param param2 TODO.
    JUTTexture(const ResTIMG* p_timg, u8 param_1) {
        mEmbPalette = NULL;
        storeTIMG(p_timg, param_1);
        setCaptureFlag(false);
    }

    /// @brief Destroys the `JUTTexture`.
    ~JUTTexture();

    void storeTIMG(const ResTIMG*, u8);
    void storeTIMG(const ResTIMG*, JUTPalette*);
    void storeTIMG(const ResTIMG*, JUTPalette*, GXTlut);
    void attachPalette(JUTPalette*);
    void init();
    void initTexObj();
    void initTexObj(_GXTlut);
    void load(_GXTexMapID);
    void capture(int, int, GXTexFmt, bool, u8);

    const ResTIMG* getTexInfo() const { return mTIMG; }
    s32 getFormat() const { return mTIMG->mFormat; }
    s32 getTransparency() const { return mTIMG->mTransparency; }
    s32 getWidth() const { return mTIMG->mWidth; }
    s32 getHeight() const { return mTIMG->mHeight; }
    void setCaptureFlag(bool flag) { mFlag &= 2 | flag; }
    bool getCaptureFlag() const { return mFlag & 1; }
    bool getEmbPaletteDelFlag() const { return mFlag & 2; }
    void setEmbPaletteDelFlag(bool flag) { mFlag = (mFlag & 1) | (flag << 1); }
    int getTlutName() const { return mTlutName; }

    /* 0x00 */ GXTexObj mObj;
    /* 0x20 */ const ResTIMG* mTIMG;
    /* 0x24 */ u8* mImage;
    /* 0x28 */ JUTPalette* mEmbPalette;
    /* 0x2C */ JUTPalette* mPalette;
    /* 0x30 */ u8 mWrapS;
    /* 0x31 */ u8 mWrapT;
    /* 0x32 */ u8 mMinType;
    /* 0x33 */ u8 mMagType;
    /* 0x34 */ u16 mMinLod;
    /* 0x36 */ u16 mMaxLod;
    /* 0x38 */ s16 mLodBias;
    /* 0x3A */ u8 mTlutName;
    /* 0x3B */ u8 mFlag;
    /* 0x3C */ ResTIMG* _3C;
};
