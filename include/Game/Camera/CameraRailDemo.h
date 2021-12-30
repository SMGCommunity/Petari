#pragma once

#include "Game/Camera/Camera.h"

class CameraRailDemo : public Camera {
public:
    CameraRailDemo(const char *);
    virtual ~CameraRailDemo();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorBase *createTranslator();

    void setParam(long, long, long, long, float);
    void calcLinear();
    void calcEaseInOut();
    void calcDamp();

    u8 _4C[24];
};