#pragma once

#include "Game/Camera/Camera.h"

class CameraCharmedFix : public Camera {
public:
    CameraCharmedFix(const char *);
    virtual ~CameraCharmedFix();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorDummy *createTranslator();

    TVec3f mAxis;   // _4C
    TVec3f mUp;     // _58
    TVec3f mWPoint; // _64
};