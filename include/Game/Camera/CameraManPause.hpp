#pragma once

#include "Game/Camera/CameraMan.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class CameraManPause : public CameraMan {
public:
    CameraManPause(const char*);
    virtual ~CameraManPause();

    virtual void init(const JMapInfoIter&);

    virtual void calc();            // TODO
    virtual void notifyActivate();  // TODO
    virtual void notifyDeactivate();
    virtual bool isCollisionOff() const;
    virtual bool isZeroFrameMoveOff() const;

    f32 _48;
    f32 _4C;
    f32 _50;
    f32 _54;
    TVec3f _58;
};