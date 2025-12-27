#pragma once

#include "Game/Camera/Camera.hpp"

class CameraRailDemo : public Camera {
public:
    CameraRailDemo(const char* pName = "レールデモカメラ");
    virtual ~CameraRailDemo();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void setParam(s32, s32, s32, s32, f32);
    void calcLinear();
    void calcEaseInOut();
    void calcDamp();

    u32* _4C;
    s32 _50;
    s32 _54;
    u32 _58;
    f32 _5C;
    f32 _60;
};