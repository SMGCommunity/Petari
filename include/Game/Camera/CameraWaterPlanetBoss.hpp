#pragma once

#include "Game/Camera/Camera.hpp"

class CameraWaterPlanetBoss : public Camera {
public:
    CameraWaterPlanetBoss(const char* pName = "水中プラネットボス");

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual bool isEnableToReset() const {
        return true;
    }
    virtual CamTranslatorBase* createTranslator();

    void setParam(f32 distMin, f32 distMax, f32 blendRateMin, bool blendUpEnabled, const TVec3f wPoint, f32 dotFrontMin, f32 dotFrontMax,
                  f32 angleXMin, f32 angleXMax) {
        mDistMin = distMin;
        mDistMax = distMax;
        mSideBlendRateMin = blendRateMin;
        mIsBlendUpEnabled = blendUpEnabled;
        mWPoint.set(wPoint);
        mDotFrontMin = dotFrontMin;
        mDotFrontMax = dotFrontMax;
        mAngleXMin = angleXMin;
        mAngleXMax = angleXMax;
    }

    /* 0x4C */ f32 mDistMin;
    /* 0x50 */ f32 mDistMax;
    /* 0x54 */ f32 mSideBlendRateMin;
    /* 0x58 */ f32 mSideBlendRate;
    /* 0x5C */ s32 mCollideCount;
    /* 0x60 */ TVec3f mLastMoveDir;
    /* 0x6C */ bool mIsBlendUpEnabled;
    /* 0x70 */ TVec3f mWPoint;
    /* 0x7C */ f32 mDotFrontMin;
    /* 0x80 */ f32 mAngleXMin;
    /* 0x84 */ f32 mDotFrontMax;
    /* 0x88 */ f32 mAngleXMax;
    /* 0x8C */ bool mIsRounding;
    /* 0x90 */ s32 mRoundingFrame;
    /* 0x94 */ TVec3f mSide;
};
