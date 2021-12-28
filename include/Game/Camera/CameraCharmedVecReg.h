#pragma once

#include "Game/Camera/Camera.h"

class CameraCharmedVecReg : public Camera {
public:
    CameraCharmedVecReg(const char *);
    virtual ~CameraCharmedVecReg();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorDummy *createTranslator();

    const char *mString;    // _4C
    f32 mDist;              // _50
    f32 mAxisX;             // _54
    f32 mAxisZ;             // _58
    f32 mAxisY;             // _5C
    f32 mAngleA;            // _60
    f32 mAngleB;            // _64
};