#pragma once

#include "Game/Camera/Camera.h"

class CameraFooFighter : public Camera {
public:
    CameraFooFighter(const char *);
    virtual ~CameraFooFighter();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorBase *createTranslator();

    f32 mAxisY; // _4C
    f32 mAxisX; // _50
    f32 mDist;  // _54
    f32 _58;
    u32 _5C;
    f32 _60;
    f32 _64;
    f32 _68;
};