#pragma once

#include "Game/Camera/Camera.h"
#include "JSystem/JGeometry/TQuat.h"

class CameraFrontAndBack : public Camera {
public:
    CameraFrontAndBack(const char *);
    virtual ~CameraFrontAndBack();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorDummy *createTranslator();

    void calcBaseQuat(TQuat4f *, const TQuat4f &);
    void calcDipQuat(TQuat4f *, const TQuat4f &, const TQuat4f &);

    TVec3f mWPoint; // _4C
    TVec3f mAxis;   // _58
    f32 mAngleA;    // _64
    f32 mAngleB;    // _68
    f32 mDist;      // _6C
};