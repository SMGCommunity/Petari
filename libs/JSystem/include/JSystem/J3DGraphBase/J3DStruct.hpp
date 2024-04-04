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

    u8 mFogType;        // _0
    u8 _1;
    u16 _2;
    f32 mStartZ;        // _4
    f32 mEndZ;          // _8
    f32 mNearZ;         // _C
    f32 mFarZ;          // _10
    _GXColor mColor;    // _14
    u16 mTable[10];     //  _18
};

struct J3DTextureSRTInfo {
    f32 mScaleX;        // _0
    f32 mScaleY;        // _4
    s16 mRotation;      // _8
    s16 mPad;           // _A
    f32 mTransX;        // _C
    f32 mTransY;        // _10
};

struct J3DTexMtxInfo {

    void operator=(const J3DTexMtxInfo &);

    u8 mProjection;                     // _0
    u8 mInfo;                           // _1
    s16 mPad;                           // _2
    TVec3f mCenter;                     // _4
    J3DTextureSRTInfo mSRTInfo;         // _10
    Mtx44 mEffectMtx;                    // _24
};

struct J3DTexMtx {
    J3DTexMtxInfo mInfo;
    Mtx mOutputMatrix;
};

static J3DTexMtxInfo j3dDefaultTexMtxInfo;

class J3DLightInfo {
public:
    Vec mLightPosition;         // _0
    Vec mLightDirection;        // _C
    GXColor mColor;             // _18
    Vec mCosAtten;              // _1C
    Vec mDistAtten;             // _28
};

class J3DLightObj {
public:
    J3DLightInfo mInfo;
    GXLightObj mLightObj;
};