#pragma once

#include "Game/Camera/Camera.h"

class CameraInnerCylinder : public Camera {
public:
    CameraInnerCylinder(const char *);
    virtual ~CameraInnerCylinder();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorDummy *createTranslator();

    TVec3f mWPoint; // _4C
    TVec3f mAxis;   // _58
    f32 mAngleA;    // _64
    f32 mAngleB;    // _68
    f32 mDist;      // _6C
    f32 mUpX;       // _70
    f32 _74;
    f32 _78;
    f32 _7C;
    bool _80;
    u8 _81[3];
};