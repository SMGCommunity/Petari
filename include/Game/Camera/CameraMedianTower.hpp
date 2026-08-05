#pragma once

#include "Game/Camera/Camera.hpp"

class CameraMedianTower : public Camera {
public:
    CameraMedianTower(const char* pName = "中点塔カメラ");

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void calcFovyPos(TVec3f*, const TVec3f&, const TVec3f&, const TVec3f&);
    void limitAngularVelocity(TVec3f*, TVec3f*, const TVec3f&, const TVec3f&, const TVec3f&);
    void calcDistance(TVec3f*, const TVec3f&);
    void updateReset(const TVec3f&, const TVec3f&, const TVec3f&);

    void setParam(const char* pName, const TVec3f& wPoint, const TVec3f& axis, f32 angle, f32 dist, f32 maxDist, f32 partialRate) {
        mString = pName;
        mWPoint.set(wPoint);
        mAxis.set(axis);
        mAngleA = angle;
        mDist = dist;
        mMaxFocusDist = maxDist;
        mPartialRate = partialRate;
    }

    /* 0x4C */ const char* mString;
    /* 0x50 */ TVec3f mWPoint;
    /* 0x5C */ TVec3f mAxis;
    /* 0x68 */ f32 mAngleA;
    /* 0x6C */ f32 mDist;
    /* 0x70 */ f32 mMaxFocusDist;
    /* 0x74 */ f32 mPartialRate;
    /* 0x78 */ TVec3f mBaseFront;
    /* 0x84 */ bool mIsResetting;
    /* 0x88 */ s32 mInterpolateTime;
};
