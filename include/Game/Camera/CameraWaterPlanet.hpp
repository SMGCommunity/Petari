#pragma once

#include "Game/Camera/Camera.hpp"

class CameraWaterPlanet : public Camera {
public:
    CameraWaterPlanet(const char *);
    virtual ~CameraWaterPlanet();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorBase *createTranslator();

    f32 mAxisX;     // _4C
    f32 mAxisY;     // _50
    f32 mAngleA;    // _54
};