#pragma once

#include "Game/Camera/Camera.h"

class CameraCharmedVecRegTower : public Camera {
public:
    CameraCharmedVecRegTower(const char *);
    virtual ~CameraCharmedVecRegTower();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorBase *createTranslator();

    TVec3f mWPoint;         // _4C
    TVec3f mAxis;           // _58
    f32 mAngleA;            // _64
    f32 mAngleB;            // _68
    f32 mDist;              // _6C
    bool _70;
    u8 _71[3];
    const char *mString;    // _74
};