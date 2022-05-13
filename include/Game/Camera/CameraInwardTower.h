#pragma once

#include "Game/Camera/Camera.h"

class CameraInwardTower : public Camera {
public:
    CameraInwardTower(const char *);
    virtual ~CameraInwardTower();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual bool isEnableToReset() const;
    virtual CamTranslatorBase *createTranslator();

    void calcIdealPose();

    TVec3f mWPoint;    // _4C
    TVec3f mAxis;      // _58
    f32 mAngleB;    // _64
    f32 mAngleA;    // _68
    f32 mDist;      // _6C
    f32 _70;
    f32 _74;
    f32 _78;
    f32 _7C;
    u8 _80;
    u8 _81[3];
};