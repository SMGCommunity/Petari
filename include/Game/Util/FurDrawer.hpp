#pragma once

#include "JSystem/JGeometry/TVec.hpp"
#include <revolution.h>

class DynamicFurParam;
class JUTTexture;
struct ResTIMG;

class FurDrawer {
public:
    class CLayerParam {
    public:
        CLayerParam(f32 tip, f32 curve) : mTip(tip), mCurve(curve) {
        }

        f32 calcValue(s32, s32) const;

        /* 0x00 */ f32 mTip;
        /* 0x04 */ f32 mRoot;
        /* 0x08 */ f32 mCurve;
    };

    FurDrawer(u32, ResTIMG*, ResTIMG*);

    void update() NO_INLINE;
    void setupMaterial(DynamicFurParam*) const;
    void setupLayerMaterial(s32) const;
    void createFurMap();

    /* 0x00 */ JUTTexture* mBodyTexture;
    /* 0x04 */ JUTTexture* mFurTexture;
    /* 0x08 */ JUTTexture* mIndirectTexture;
    /* 0x0C */ s32 mNumLayers;
    /* 0x10 */ CLayerParam mLength;
    /* 0x1C */ CLayerParam mIndirect;
    /* 0x28 */ CLayerParam mBrightness;
    /* 0x34 */ CLayerParam mAlpha;
    /* 0x40 */ f32 mFurUVScale;
    /* 0x44 */ u32 mMixFog;
    /* 0x48 */ f32 mBodyUVScale;
    /* 0x4C */ f32 mDensity[4];
    /* 0x5C */ f32 mThickness[4];
    /* 0x6C */ u8 mLengthMap[4];
    /* 0x70 */ CLayerParam mColorBlend;
    /* 0x7C */ GXColor mColor;
    /* 0x80 */ u8 mCullMode;
    /* 0x81 */ u8 mMatIntensity;
    /* 0x82 */ u8 mAmbIntensity;
    /* 0x83 */ u8 mUseIndirect;
    /* 0x84 */ u8 mZCompLoc;
    /* 0x85 */ u8 mZWrite;
    /* 0x86 */ u8 mAdditive;
    /* 0x87 */ u8 mAlphaRef;
    /* 0x88 */ Mtx mFurTexMtx;
    /* 0xB8 */ Mtx mIndirectTexMtx;
    /* 0xE8 */ TVec3f mFogPosition;
    /* 0xF4 */ f32 mFogWidth;
    /* 0xF8 */ u8 mUseDensityMap;
};
