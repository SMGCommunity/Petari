#pragma once

#include "Game/Camera/Camera.hpp"

class CameraCubePlanet : public Camera {
public:
    CameraCubePlanet(const char *);
    virtual ~CameraCubePlanet();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorBase *createTranslator();

    f32 mDist;      // _4C
    f32 mAngleA;    // _50
    f32 mAngleB;    // _54
    f32 _58;
    f32 _5C;
    f32 _60;
};