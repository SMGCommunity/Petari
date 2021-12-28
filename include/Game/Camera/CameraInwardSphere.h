#pragma once

#include "Game/Camera/Camera.h"

class CameraInwardSphere : public Camera {
public:
    CameraInwardSphere(const char *);
    virtual ~CameraInwardSphere();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorDummy *createTranslator();

    void calcGravityCenter();

    f32 mDist;      // _4C
    f32 mAngleA;    // _50
    f32 mAngleB;    // _54
    f32 _58;
    f32 _5C;
    f32 _60;
    f32 _64;
    f32 _68;
    f32 _6C;
    u8 _70[12];
};