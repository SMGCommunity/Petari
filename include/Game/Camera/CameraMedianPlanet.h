#pragma once

#include "Game/Camera/Camera.h"
#include "JSystem/JGeometry/TPosition3.h"

class CameraMedianPlanet : public Camera {
public:
    CameraMedianPlanet(const char *);
    virtual ~CameraMedianPlanet();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorDummy *createTranslator();

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

    u32 _4C;
    f32 _50;
    f32 _54;
    f32 _58;
    f32 _5C;
    f32 _60;
    f32 _64;
    f32 _68;
};