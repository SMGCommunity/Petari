#pragma once

#include "Game/Camera/Camera.hpp"

class CameraFooFighter : public Camera {
public:
    CameraFooFighter(const char* pName = "フーファイター");
    virtual ~CameraFooFighter();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    f32 mAxisY;  // 0x4C
    f32 mAxisX;  // 0x50
    f32 mDist;   // 0x54
    f32 _58;
    u32 _5C;
    f32 _60;
    f32 _64;
    f32 _68;
};