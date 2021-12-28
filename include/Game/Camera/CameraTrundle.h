#pragma once

#include "Game/Camera/Camera.h"

class CameraTrundle : public Camera {
public:
    CameraTrundle(const char *);
    virtual ~CameraTrundle();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorDummy *createTranslator();

    TVec3f mWPoint;    // _4C
    TVec3f mAxis;      // _58
    f32 mDist;      // _64
    f32 mAngleA;    // _68
    f32 mAngleB;    // _6C
    f32 mUpX;       // _70
};