#pragma once

#include <revolution/gx/GXStruct.h>
#include <revolution/types.h>

class FogCtrl;

class FurParam {
public:
    /* 0x00 */ s32 mNumLayers;
    /* 0x04 */ f32 mLength;
    /* 0x08 */ f32 mLengthCurve;
    /* 0x0C */ f32 mIndirect;
    /* 0x10 */ f32 mIndirectCurve;
    /* 0x14 */ f32 mBrightnessTip;
    /* 0x18 */ f32 mBrightnessRoot;
    /* 0x1C */ f32 mBrightnessCurve;
    /* 0x20 */ f32 mAlphaTip;
    /* 0x24 */ f32 mAlphaRoot;
    /* 0x28 */ f32 mAlphaCurve;
    /* 0x2C */ f32 mSkinAlphaTip;
    /* 0x30 */ f32 mSkinAlphaRoot;
    /* 0x34 */ f32 mSkinAlphaCurve;
    /* 0x38 */ f32 mFurUVScale;
    /* 0x3C */ f32 mBodyUVScale;
    /* 0x40 */ GXColor mColor;
    /* 0x44 */ f32 mDensity[4];
    /* 0x54 */ f32 mThickness[4];
    /* 0x64 */ u8 mLengthMap[4];
};

class FurLightParam {
public:
    FurLightParam()
        : mLightMask(1), mSpecularLightMask(), mMatIntensity(255), mAmbIntensity(50), mSpecularMatIntensity(255), mSpecularAmbIntensity(),
          mLightType(-1) {
        mColorSource = 0;
    }

    /* 0x00 */ u16 mLightMask;
    /* 0x02 */ u16 mSpecularLightMask;
    /* 0x04 */ u8 mMatIntensity;
    /* 0x05 */ u8 mAmbIntensity;
    /* 0x06 */ u8 mSpecularMatIntensity;
    /* 0x07 */ u8 mSpecularAmbIntensity;
    /* 0x08 */ u8 mColorSource;
    /* 0x0C */ s32 mLightType;
};

class DynamicFurParam {
public:
    /* 0x00 */ FogCtrl* mFogCtrl;
    /* 0x04 */ FurLightParam* mLightParam;
};

namespace MR {
    void initFurParamFromDvd(FurParam*, DynamicFurParam*, char*, u32);
}
