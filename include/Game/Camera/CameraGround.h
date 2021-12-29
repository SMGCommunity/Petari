#pragma once

#include "Game/Camera/Camera.h"

class CameraGround : public Camera {
public:
    CameraGround(const char *);
    virtual ~CameraGround();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorBase *createTranslator();

    f32 mAngleA;    // _4C
    f32 mAngleB;    // _50
    f32 mDist;      // _54
    f32 mUpX;       // _58
    f32 mUpY;       // _5C
    f32 mUpZ;       // _60
    f32 _64;
    f32 _68;
    f32 _6C;
    bool _70;
    u8 _71[3];
};