#pragma once

#include "Game/Camera/Camera.h"

class CameraFooFighterPlanet : public Camera {
public:
    CameraFooFighterPlanet(const char *);

    virtual CameraTargetObj *calc();

    f32 _4C;
    f32 _50;
    f32 _54;
};