#pragma once

#include "Game/Camera/Camera.h"

class CameraMedianTower : public Camera {
public:
    CameraMedianTower(const char *);
    virtual ~CameraMedianTower();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorDummy *createTranslator();

    void calcFovyPos(TVec3f *, const TVec3f &, const TVec3f &, const TVec3f &);
    void limitAngularVelocity(TVec3f *, TVec3f *, const TVec3f &, const TVec3f &, const TVec3f &);
    void calcDistance(TVec3f *, const TVec3f &);
    void updateReset(const TVec3f &, const TVec3f &, const TVec3f &);

    const char *mString;    // _4C
    TVec3f mWPoint;         // _50
    TVec3f mAxis;           // _5C
    f32 mAngleA;            // _68
    f32 mUpX;               // _6C
    f32 mUpY;               // _70
    f32 mUpZ;               // _74
    f32 _78;
    f32 _7C;
    f32 _80;
    u8 _84;
    u8 _85[3];
    u32 _88;
};