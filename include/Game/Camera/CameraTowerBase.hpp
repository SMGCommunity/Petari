#pragma once

#include "Game/Camera/Camera.hpp"

class CameraTowerBase : public Camera {
public:
    CameraTowerBase(const char*);
    virtual ~CameraTowerBase();

    virtual void reset();
    virtual CameraTargetObj* calc();

    virtual void resetBegin() {
    }
    virtual void resetPreVPan() {
    }
    virtual void resetPostVPan() {
    }
    virtual void arrangePreVPan() {
    }
    virtual void arrangePostVPan() {
    }

    void calcIdealPoseOfTowerBase();

    /* 0x4C */ TVec3f mWPoint;
    /* 0x58 */ TVec3f mAxis;
    /* 0x64 */ f32 mAngleX;
    /* 0x68 */ f32 mDist;
};
