#pragma once

#include "Game/Camera/CameraMan.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class CameraManPause : public CameraMan {
public:
    CameraManPause(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);

    /* 0x24 */ virtual void calc();
    /* 0x28 */ virtual void notifyActivate();
    /* 0x2C */ virtual void notifyDeactivate();
    /* 0x34 */ virtual bool isCollisionOff() const {
        return true;
    }
    /* 0x38 */ virtual bool isZeroFrameMoveOff() const {
        return true;
    }

    /* 0x48 */ f32 mDistBase;
    /* 0x4C */ f32 mDistOffset;
    /* 0x50 */ TVec2f mAngle;
    /* 0x58 */ TVec3f mBasePos;
};
