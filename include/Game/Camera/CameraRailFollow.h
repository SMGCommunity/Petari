#pragma once

#include "Game/Camera/Camera.h"

class CameraRailFollow : public Camera {
public:
    CameraRailFollow(const char *);
    virtual ~CameraRailFollow();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorDummy *createTranslator();

    void setParam(long, long, float, float, float, long);

    u32 _4C;
    f32 _50;
    f32 _54;
    f32 _58;
    f32 _5C;
    f32 _60;
    u8 _64[4];
};