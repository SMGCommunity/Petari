#pragma once

#include "Game/Camera/Camera.hpp"

class RailRider;

class CameraRailFollow : public Camera {
public:
    CameraRailFollow(const char* pName = "レールフォローカメラ");

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void setParam(s32, s32, f32, f32, f32, s32);

    f32 getStopLength() {
        return mRailSpeed * mRailSpeed / (mRailAccel * 2.0f);
    }

    /* 0x4C */ RailRider* mRailRider;
    /* 0x50 */ f32 mRailCoord;
    /* 0x54 */ f32 mRailSpeed;
    /* 0x58 */ f32 mTargetMargin;
    /* 0x5C */ f32 mRailSpeedMax;
    /* 0x60 */ f32 mRailAccel;
    /* 0x64 */ s32 mMarginScale;
};
