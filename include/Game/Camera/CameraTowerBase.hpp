#pragma once

#include "Game/Camera/Camera.hpp"

class CameraTowerBase : public Camera {
public:
    CameraTowerBase(const char*);
    /* 0x08 */ virtual ~CameraTowerBase();

    /* 0x24 */ virtual void reset();
    /* 0x28 */ virtual CameraTargetObj* calc();

    /* 0x58 */ virtual void resetBegin() {
    }
    /* 0x5C */ virtual void resetPreVPan() {
    }
    /* 0x60 */ virtual void resetPostVPan() {
    }
    /* 0x64 */ virtual void arrangePreVPan() {
    }
    /* 0x68 */ virtual void arrangePostVPan() {
    }

    void calcIdealPoseOfTowerBase();

    /* 0x4C */ TVec3f mWPoint;
    /* 0x58 */ TVec3f mAxis;
    /* 0x64 */ f32 mAngleX;
    /* 0x68 */ f32 mDist;
};
