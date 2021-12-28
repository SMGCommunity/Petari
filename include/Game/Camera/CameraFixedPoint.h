#pragma once

#include "Game/Camera/Camera.h"

class CameraFixedPoint : public Camera {
public:
    CameraFixedPoint(const char *);
    virtual ~CameraFixedPoint();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorDummy *createTranslator();

    void setParam(const TVec3f &, unsigned long);

    f32 _4C;
    f32 _50;
    f32 _54;
    u8 _58[4];
};