#pragma once

#include "Game/Camera/Camera.h"

class CameraTripodPlanet : public Camera {
public:
    CameraTripodPlanet(const char *);
    virtual ~CameraTripodPlanet();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorDummy *createTranslator();

    f32 _4C;
    f32 _50;
    f32 _54;
    f32 _58;
    f32 _5C;
    f32 _60;
    f32 _64;
    f32 _68;
    f32 _6C;
    u8 _70[4];
};