#pragma once

#include "Game/Camera/Camera.hpp"

class CameraRailWatch : public Camera {
public:
    CameraRailWatch(const char *);
    virtual ~CameraRailWatch();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorBase *createTranslator();

    void setParam(long, long, long, float, float, float);

    u32 _4C;
    u32 _50;
    u32 _54;
    f32 _58;
    f32 _5C;
    f32 _60;
};