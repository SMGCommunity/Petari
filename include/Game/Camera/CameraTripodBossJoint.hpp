#pragma once

#include "Game/Camera/Camera.h"

class CameraTripodBossJoint : public Camera {
public:
    CameraTripodBossJoint(const char *);
    virtual ~CameraTripodBossJoint();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual bool isEnableToReset() const;
    virtual CamTranslatorBase *createTranslator();

    void calcIdealPose();

    f32 mAngleB;    // _4C
    f32 mAngleA;    // _50
    f32 mDist;      // _54
    s32 _58;
    f32 mAxisX;     // _5C
    f32 mAxisY;     // _60
    f32 mAxisZ;     // _64
    bool _68;
    u8 _69[3];
    f32 _6C;
    f32 _70;
};