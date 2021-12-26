#pragma once

#include "Game/Camera/Camera.h"

class CameraWaterPlanet : public Camera {
public:
    CameraWaterPlanet(const char *);
    virtual ~CameraWaterPlanet();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorDummy *createTranslator();

    f32 _4C;
    f32 _50;
    f32 _54;
};