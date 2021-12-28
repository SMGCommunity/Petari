#pragma once

#include "Game/Camera/Camera.h"

class CameraObjParallel : public Camera {
public:
    CameraObjParallel(const char *);
    virtual ~CameraObjParallel();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorDummy *createTranslator();

    f32 _4C;
    f32 _50;
    f32 _54;
};