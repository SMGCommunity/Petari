#pragma once

#include "Game/Camera/Camera.hpp"

class RailRider;

class CameraRailDemo : public Camera {
public:
    CameraRailDemo(const char* pName = "レールデモカメラ");

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void setParam(s32, s32, s32, s32, f32);
    void calcLinear();
    void calcEaseInOut();
    void calcDamp();

    /* 0x4C */ RailRider* mRailRider;
    /* 0x50 */ s32 mCalcType;
    /* 0x54 */ s32 mDemoTime;
    /* 0x58 */ s32 mDemoTimer;
    /* 0x5C */ f32 mDampRatio;
    /* 0x60 */ f32 mCoord;
};
