#pragma once

#include "Game/Camera/Camera.hpp"

class CameraTalk : public Camera {
public:
    CameraTalk(const char* pName = "会話カメラ");
    virtual ~CameraTalk();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void setParam(const TVec3f&, const TVec3f&, f32, f32);

    f32 _4C;
    f32 _50;
    f32 _54;
    f32 _58;
    f32 _5C;
    f32 _60;
    f32 _64;
    f32 _68;
    u8 _6C;
    u8 _6D[3];
};