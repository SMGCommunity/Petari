#pragma once

#include "Game/Camera/Camera.hpp"

class CameraBehind : public Camera {
public:
    CameraBehind(const char* pName = "後方カメラ");
    virtual ~CameraBehind();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void calcPosAndUp(f32 accel);

    void setParam(const TVec2f& angle, f32 dist, f32 rate) {
        mAngleX = angle.x;
        mAngleY = angle.y;
        mDist = dist;
        mRate = rate;
    }

    /* 0x4C */ f32 mAngleX;
    /* 0x50 */ f32 mAngleY;
    /* 0x54 */ f32 mDist;
    /* 0x58 */ f32 mRate;
};
