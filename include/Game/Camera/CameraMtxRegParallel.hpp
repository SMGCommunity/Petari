#pragma once

#include "Game/Camera/Camera.hpp"

class CameraMtxRegParallel : public Camera {
public:
    CameraMtxRegParallel(const char *);
    virtual ~CameraMtxRegParallel();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorBase *createTranslator();

    void calcIdealPose();

    const char *mString;    // _4C
    f32 mAngleB;            // _50
    f32 mAngleA;            // _54
    f32 mDist;              // _58
    TVec3f mWPoint;         // _5C
};