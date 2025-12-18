#pragma once

#include "Game/Camera/Camera.hpp"

class CameraRailWatch : public Camera {
public:
    CameraRailWatch(const char* pName = "レール注目カメラ");
    virtual ~CameraRailWatch();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void setParam(s32, s32, s32, f32, f32, f32);

    u32 _4C;
    u32 _50;
    u32 _54;
    f32 _58;
    f32 _5C;
    f32 _60;
};