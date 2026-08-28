#pragma once

#include "Game/Camera/Camera.hpp"

class CameraParallel : public Camera {
public:
    CameraParallel(const char* pName = "平行カメラ");

    /* 0x24 */ virtual void reset();
    /* 0x28 */ virtual CameraTargetObj* calc();
    /* 0x54 */ virtual CamTranslatorBase* createTranslator();
    /* 0x48 */ virtual bool isEnableToRoundLeft() const;
    /* 0x4C */ virtual bool isEnableToRoundRight() const;
    /* 0x50 */ virtual bool isEnableToReset() const;

    void calcIdealPose();
    void calcRound();

    void setParam(const TVec2f& angle, f32 dist, bool enabled) {
        mAngleX = angle.x;
        mAngleY = angle.y;
        mDist = dist;
        mIsEnableValid = enabled;
    }

    /* 0x4C */ f32 mAngleX;
    /* 0x50 */ f32 mAngleY;
    /* 0x54 */ f32 mDist;
    /* 0x58 */ bool mIsEnableValid;
    /* 0x59 */ bool mIsRounding;
    /* 0x5C */ f32 mRoundTarget;
    /* 0x60 */ f32 mRoundAngle;
    /* 0x64 */ f32 mRoundAddition;
};
