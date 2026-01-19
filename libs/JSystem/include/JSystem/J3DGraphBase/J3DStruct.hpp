#pragma once

#include <JSystem/JGeometry/TMatrix.hpp>
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution.h>

class J3DGXColor {
public:
    J3DGXColor& operator=(_GXColor);

    u8 r;
    u8 g;
    u8 b;
    u8 a;
};

struct J3DFogInfo {
    J3DFogInfo& operator=(J3DFogInfo const&);

    /* 0x00 */ u8 mType;
    /* 0x01 */ u8 mAdjEnable;
    /* 0x02 */ u16 mCenter;
    /* 0x04 */ f32 mStartZ;
    /* 0x08 */ f32 mEndZ;
    /* 0x0C */ f32 mNearZ;
    /* 0x10 */ f32 mFarZ;
    /* 0x14 */ GXColor mColor;
    /* 0x18 */ GXFogAdjTable mFogAdjTable;
};  // Size: 0x2C

struct J3DBlendInfo {
    void operator=(J3DBlendInfo const& other) { __memcpy(this, &other, sizeof(J3DBlendInfo)); }
    /* 0x0 */ u8 mType;
    /* 0x1 */ u8 mSrcFactor;
    /* 0x2 */ u8 mDstFactor;
    /* 0x3 */ u8 mOp;
};

struct J3DTextureSRTInfo {
    /* 0x00 */ f32 mScaleX;
    /* 0x04 */ f32 mScaleY;
    /* 0x08 */ s16 mRotation;
    /* 0x0C */ f32 mTranslationX;
    /* 0x10 */ f32 mTranslationY;

    inline void operator=(J3DTextureSRTInfo const& other) {
#ifdef __MWERKS__
        __REGISTER const f32* src = &other.mScaleX;
        __REGISTER f32* dst = &mScaleX;
        __REGISTER f32 xy;
        asm {
            psq_l xy, 0(src), 0, 0
            psq_st xy, 0(dst), 0, 0
        }
        ;

        // Unclear why there's a 4 byte copy here.
        *(u32*)&mRotation = *(u32*)&other.mRotation;
        src = &other.mTranslationX;
        dst = &mTranslationX;

        asm {
            psq_l xy, 0(src), 0, 0
            psq_st xy, 0(dst), 0, 0
        }
        ;
#endif
    }
};  // Size: 0x14

struct J3DTexMtxInfo {
    void operator=(const J3DTexMtxInfo&);

    u8 mProjection;              // 0x0
    u8 mInfo;                    // 0x1
    s16 mPad;                    // 0x2
    TVec3f mCenter;              // 0x4
    J3DTextureSRTInfo mSRTInfo;  // 0x10
    Mtx44 mEffectMtx;            // 0x24
};

struct J3DTexMtx {
    J3DTexMtxInfo mInfo;
    Mtx mOutputMatrix;
};

class J3DLightInfo {
public:
    Vec mLightPosition;   // 0x0
    Vec mLightDirection;  // 0xC
    GXColor mColor;       // 0x18
    Vec mCosAtten;        // 0x1C
    Vec mDistAtten;       // 0x28
};

struct J3DColorChanInfo {
    /* 0x0 */ u8 mEnable;
    /* 0x1 */ u8 mMatSrc;
    /* 0x2 */ u8 mLightMask;
    /* 0x3 */ u8 mDiffuseFn;
    /* 0x4 */ u8 mAttnFn;
    /* 0x5 */ u8 mAmbSrc;
    /* 0x6 */ u8 pad[2];
};

class J3DLightObj {
public:
    J3DLightInfo mInfo;
    GXLightObj mLightObj;
};

struct J3DTexCoordInfo {
    /* 0x0 */ u8 mTexGenType;
    /* 0x1 */ u8 mTexGenSrc;
    /* 0x2 */ u8 mTexGenMtx;
    /* 0x3 */ u8 pad;

    J3DTexCoordInfo& operator=(const J3DTexCoordInfo& other) {
        __memcpy(this, &other, sizeof(J3DTexCoordInfo));
        return *this;
    }
};

struct J3DNBTScaleInfo {
    J3DNBTScaleInfo& operator=(J3DNBTScaleInfo const&);

    /* 0x0 */ u8 mbHasScale;
    /* 0x4 */ Vec mScale;
};  // Size: 0x10

struct J3DTevOrderInfo {
    void operator=(const J3DTevOrderInfo& other) { *(u32*)this = *(u32*)&other; }

    /* 0x0 */ u8 mTexCoord;
    /* 0x1 */ u8 mTexMap;
    /* 0x2 */ u8 mColorChan;
    /* 0x3 */ u8 field_0x3;  // Maybe padding
};

struct J3DTevSwapModeTableInfo {
    /* 0x0 */ u8 field_0x0;
    /* 0x1 */ u8 field_0x1;
    /* 0x2 */ u8 field_0x2;
    /* 0x3 */ u8 field_0x3;
};  // Size: 0x4

struct J3DTevStageInfo {
    /* 0x0 */ u8 field_0x0;
    /* 0x1 */ u8 mTevColorOp;
    /* 0x2 */ u8 mTevColorAB;
    /* 0x3 */ u8 mTevColorCD;
    /* 0x4 */ u8 field_0x4;
    /* 0x5 */ u8 mTevAlphaOp;
    /* 0x6 */ u8 mTevAlphaAB;
    /* 0x7 */ u8 mTevSwapModeInfo;
    /* 0x8 */ u8 field_0x8;
    /* 0x8 */ u8 field_0x9;
    /* 0x8 */ u8 field_0xa;
    /* 0x8 */ u8 field_0xb;
    /* 0x8 */ u8 field_0xc;
    /* 0x8 */ u8 field_0xd;
    /* 0x8 */ u8 field_0xe;
    /* 0x8 */ u8 field_0xf;
    /* 0x8 */ u8 field_0x10;
    /* 0x8 */ u8 field_0x11;
    /* 0x8 */ u8 field_0x12;
    /* 0x8 */ u8 field_0x13;
};

struct J3DIndTevStageInfo {
    /* 0x0 */ u8 mIndStage;
    /* 0x1 */ u8 mIndFormat;
    /* 0x2 */ u8 mBiasSel;
    /* 0x3 */ u8 mMtxSel;
    /* 0x4 */ u8 mWrapS;
    /* 0x5 */ u8 mWrapT;
    /* 0x6 */ u8 mPrev;
    /* 0x7 */ u8 mLod;
    /* 0x8 */ u8 mAlphaSel;
    /* 0x9 */ u8 pad[3];
};

struct J3DTevSwapModeInfo {
    /* 0x0 */ u8 mRasSel;
    /* 0x1 */ u8 mTexSel;
    /* 0x2 */ u8 field_0x2;
    /* 0x3 */ u8 field_0x3;
};  // Size: 0x4

struct J3DIndTexOrderInfo {
    /* 0x0 */ u8 mCoord;
    /* 0x1 */ u8 mMap;
    /* 0x2 */ u8 field_0x2;
    /* 0x3 */ u8 field_0x3;

    void operator=(J3DIndTexOrderInfo const& other) { __memcpy(this, &other, sizeof(J3DIndTexOrderInfo)); }
};  // Size: 0x04

struct J3DIndTexMtxInfo {
    J3DIndTexMtxInfo& operator=(J3DIndTexMtxInfo const&);
    /* 0x00 */ Mtx23 field_0x0;
    /* 0x18 */ u8 field_0x18;
};  // Size: 0x1C

struct J3DIndTexCoordScaleInfo {
    /* 0x0 */ u8 mScaleS;
    /* 0x1 */ u8 mScaleT;
    /* 0x2 */ u8 field_0x2;
    /* 0x3 */ u8 field_0x3;

    J3DIndTexCoordScaleInfo& operator=(const J3DIndTexCoordScaleInfo& other) {
        __memcpy(this, &other, sizeof(J3DIndTexCoordScaleInfo));
        return *this;
    }
};  // Size: 0x4

struct J3DAlphaCompInfo {
    /* 0x0 */ u8 mComp0;
    /* 0x1 */ u8 mRef0;
    /* 0x2 */ u8 mOp;
    /* 0x3 */ u8 mComp1;
    /* 0x4 */ u8 mRef1;
    /* 0x5 */ u8 field_0x5;
    /* 0x6 */ u8 field_0x6;
    /* 0x7 */ u8 field_0x7;

    J3DAlphaCompInfo& operator=(const J3DAlphaCompInfo& other) {
        mComp0 = other.mComp0;
        mRef0 = other.mRef0;
        mOp = other.mOp;
        mComp1 = other.mComp1;
        mRef1 = other.mRef1;
        return *this;
    }
};
