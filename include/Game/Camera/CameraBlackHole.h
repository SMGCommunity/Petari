#pragma once

#include "Game/Camera/Camera.h"

class CameraBlackHole : public Camera {
public:
    CameraBlackHole(const char *);
    virtual ~CameraBlackHole();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorDummy *createTranslator();

    f32 _4C;
    f32 _50;
    TVec3f mWPoint; // _54
    TVec3f mAxis;   // _60
};