#pragma once

#include "Game/Camera/Camera.h"

class CameraSpiral : public Camera {
public:
    CameraSpiral(const char *);
    virtual ~CameraSpiral();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorDummy *createTranslator();

    s32 _4C;
    u32 _50;
    s32 _54;
    s32 _58;
    f32 mWPointY;   // _5C
    f32 mAxisY;     // _60
    f32 mWPointZ;   // _64
    f32 mAxisZ;     // _68
    f32 mWPointX;   // _6C
    f32 mAxisX;     // _70
};