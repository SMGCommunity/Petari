#pragma once

#include "Game/Camera/Camera.h"

class CameraFooFighter : public Camera {
public:
    CameraFooFighter(const char *);
    virtual ~CameraFooFighter();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorDummy *createTranslator();

    f32 _4C;
    f32 _50;
    f32 _54;
    f32 _58;
    u32 _5C;
    f32 _60;
    f32 _64;
    f32 _68;
};