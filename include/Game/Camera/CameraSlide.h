#pragma once

#include "Game/Camera/Camera.h"

class CameraSlide : public Camera {
public:
    CameraSlide(const char *);
    virtual ~CameraSlide();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorDummy *createTranslator();

    TVec3f mAxis;   // _4C
    TVec3f mUp;     // _58
    TVec3f mWPoint; // _64
    f32 mAngleA;    // _70
    f32 mDist;      // _74
};