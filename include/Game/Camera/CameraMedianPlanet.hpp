#pragma once

#include "Game/Camera/Camera.h"
#include "JSystem/JGeometry.h"

class CameraMedianPlanet : public Camera {
public:
    CameraMedianPlanet(const char *);
    virtual ~CameraMedianPlanet();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorBase *createTranslator();

    void getPrevFront(TVec3f *);
    void constrainDipAngle(TVec3f *);
    void makeTargetMtx(TPos3f *);
    void peepMtx(TPos3f *);
    void rotate33(TPos3f *, const TVec3f &, float);
    void constrainLength(TVec3f *);
    void getMaxRate();
    void getRegisterUpVec(TVec3f *);
    void calcWatchPos(TVec3f *);
    void calcUpVec(TVec3f *, const TVec3f &, const TVec3f &);

    const char *mString;    // _4C
    f32 mAxisX;             // _50
    f32 mAxisY;             // _54
    f32 mAngleA;            // _58
    f32 mAngleB;            // _5C
    f32 mDist;              // _60
    f32 mAxisZ;             // _64
    f32 mWPointX;           // _68
};