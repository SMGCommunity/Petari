#pragma once

#include "Game/Camera/Camera.h"

class CameraTripodPlanet : public Camera {
public:
    CameraTripodPlanet(const char *);
    virtual ~CameraTripodPlanet();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorBase *createTranslator();

    TVec3f mAxis;   // _4C
    TVec3f mWPoint; // _58
    f32 mAngleA;    // _64
    f32 mAngleB;    // _68
    f32 mDist;      // _6C
    u8 _70[4];
};