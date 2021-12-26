#pragma once

#include "Game/Camera/Camera.h"

class CameraCharmedVecReg : public Camera {
public:
    CameraCharmedVecReg(const char *);
    virtual ~CameraCharmedVecReg();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorDummy *createTranslator();

    u32 _4C;
    f32 _50;
    f32 _54;
    f32 _58;
    f32 _5C;
    f32 _60;
    f32 _64;
};