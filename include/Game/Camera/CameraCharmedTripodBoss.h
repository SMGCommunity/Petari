#pragma once

#include "Game/Camera/Camera.h"

class CameraCharmedTripodBoss : public Camera {
public:
    CameraCharmedTripodBoss(const char *);
    virtual ~CameraCharmedTripodBoss();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorBase *createTranslator();

    void setParam(long, TVec3f, const TVec3f &, const TVec2f &);

    s32 _4C;
    f32 _50;
    f32 _54;
    f32 _58;
    f32 _5C;
    f32 _60;
    f32 _64;
    f32 _68;
    f32 _6C;
};