#pragma once

#include "Game/Camera/CameraMan.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class CameraManPause : public CameraMan {
public:
    CameraManPause(const char*);

    virtual void init(const JMapInfoIter&);

    virtual void calc();
    virtual void notifyActivate();
    virtual void notifyDeactivate();
    virtual bool isCollisionOff() const {
        return true;
    }
    virtual bool isZeroFrameMoveOff() const {
        return true;
    }

    /* 0x48 */ f32 mDistBase;
    /* 0x4C */ f32 mDistOffset;
    /* 0x50 */ TVec2f mAngle;
    /* 0x58 */ TVec3f mBasePos;
};
