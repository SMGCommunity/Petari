#pragma once

#include "Game/Camera/Camera.hpp"

class CameraTowerBase : public Camera {
public:
    CameraTowerBase(const char*);
    virtual ~CameraTowerBase();

    virtual void reset();
    virtual CameraTargetObj* calc();

    void calcIdealPoseOfTowerBase();
    void resetBegin();
    void resetPreVPan();
    void resetPostVPan();
    void arrangePreVPan();
    void arrangePostVPan();

    f32 _4C;
    f32 _50;
    f32 _54;
    f32 _58;
    f32 _5C;
    f32 _60;
    f32 _64;
    f32 _68;
};