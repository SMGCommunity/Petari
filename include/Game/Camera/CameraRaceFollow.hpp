#pragma once

#include "Game/Camera/Camera.hpp"

class CameraRaceFollow : public Camera {
public:
    CameraRaceFollow(const char* pName = "CameraRaceFollow");

    /* 0x24 */ virtual void reset();
    /* 0x28 */ virtual CameraTargetObj* calc();
    /* 0x54 */ virtual CamTranslatorBase* createTranslator();

    void goRound();

    void setParam(f32 distMin, f32 distMax, f32 angle, bool forcePitch, f32 roundSpeed) {
        mDistMin = distMin;
        mDistMax = distMax;
        mAngleX = angle;
        mIsForcePitch = forcePitch;
        mRoundAngleSpeed = roundSpeed;
    }

    /* 0x4C */ f32 mDistMin;
    /* 0x50 */ f32 mDistMax;
    /* 0x54 */ f32 mAngleX;
    /* 0x58 */ bool mIsForcePitch;
    /* 0x5C */ f32 mRoundAngleSpeed;
};
