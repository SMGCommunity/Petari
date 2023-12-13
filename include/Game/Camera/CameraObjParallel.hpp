#pragma once

#include "Game/Camera/Camera.hpp"

class CameraObjParallel : public Camera {
public:
    CameraObjParallel(const char *);
    virtual ~CameraObjParallel();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorBase *createTranslator();

    f32 mAngleA;    // _4C
    f32 mAngleB;    // _50
    f32 mDist;      // _54
};