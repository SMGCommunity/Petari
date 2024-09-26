#pragma once

#include <revolution.h>
#include <JSystem/JGeometry/TMatrix.hpp>
#include <JSystem/JGeometry/TVec.hpp>

class J3DGXColor {
public:
    J3DGXColor& operator=(_GXColor);

    u8 r;
    u8 g;
    u8 b;
    u8 a;
};

class J3DFogInfo {
public:
    inline J3DFogInfo() {
        
    }

    J3DFogInfo& operator=(const J3DFogInfo &);

    u8 mFogType;        // 0x0
    u8 _1;
    u16 _2;
    f32 mStartZ;        // 0x4
    f32 mEndZ;          // 0x8
    f32 mNearZ;         // 0xC
    f32 mFarZ;          // 0x10
    _GXColor mColor;    // 0x14
    u16 mTable[10];     //  _18
};

struct J3DTextureSRTInfo {
    f32 mScaleX;        // 0x0
    f32 mScaleY;        // 0x4
    s16 mRotation;      // 0x8
    s16 mPad;           // 0xA
    f32 mTransX;        // 0xC
    f32 mTransY;        // 0x10
};

struct J3DTexMtxInfo {

    void operator=(const J3DTexMtxInfo &);

    u8 mProjection;                     // 0x0
    u8 mInfo;                           // 0x1
    s16 mPad;                           // 0x2
    TVec3f mCenter;                     // 0x4
    J3DTextureSRTInfo mSRTInfo;         // 0x10
    Mtx44 mEffectMtx;                    // 0x24
};

struct J3DTexMtx {
    J3DTexMtxInfo mInfo;
    Mtx mOutputMatrix;
};

static J3DTexMtxInfo j3dDefaultTexMtxInfo;

class J3DLightInfo {
public:
    Vec mLightPosition;         // 0x0
    Vec mLightDirection;        // 0xC
    GXColor mColor;             // 0x18
    Vec mCosAtten;              // 0x1C
    Vec mDistAtten;             // 0x28
};

class J3DLightObj {
public:
    J3DLightInfo mInfo;
    GXLightObj mLightObj;
};