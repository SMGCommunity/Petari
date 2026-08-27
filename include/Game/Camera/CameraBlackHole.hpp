#pragma once

#include "Game/Camera/Camera.hpp"

class CameraBlackHole : public Camera {
public:
    CameraBlackHole(const char* pName = "ブラックホールカメラ");

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void setParam(const TVec3f rWPoint, const TVec3f rBasePos) {
        mWPoint.set(rWPoint);
        mBasePos.set(rBasePos);
    }

    /* 0x4C */ f32 mFovy;
    /* 0x50 */ f32 mRoll;
    /* 0x54 */ TVec3f mWPoint;
    /* 0x60 */ TVec3f mBasePos;
};
