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

    f32 _4C;
    f32 _50;
    f32 _54;
    f32 _58;
    f32 _5C;
    f32 _60;
    f32 _64;
    f32 _68;
    f32 _6C;
};