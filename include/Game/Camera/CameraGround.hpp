#pragma once

#include "Game/Camera/Camera.hpp"

class CameraGround : public Camera {
public:
    CameraGround(const char* pName = "地面カメラ");

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void setParam(const TVec2f& angle, f32 dist, const TVec3f& rUp) {
        mAngleA = angle.x;
        mAngleB = angle.y;
        mDist = dist;
        mUp.set(rUp);
    }

    /* 0x4C */ f32 mAngleA;
    /* 0x50 */ f32 mAngleB;
    /* 0x54 */ f32 mDist;
    /* 0x58 */ TVec3f mUp;
    /* 0x64 */ TVec3f mLastWatchPoint;
    /* 0x70 */ bool mIsNotInitialized;
};
