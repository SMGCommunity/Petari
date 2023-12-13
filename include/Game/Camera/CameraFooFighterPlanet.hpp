#pragma once

#include "Game/Camera/Camera.hpp"

class CameraFooFighterPlanet : public Camera {
public:
    CameraFooFighterPlanet(const char *);
    virtual ~CameraFooFighterPlanet();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorBase *createTranslator();

    void goRoundBehind(TVec3f &, TVec3f &, TVec3f &);

    f32 mAxisX;     // _4C
    f32 mAxisY;     // _50
    f32 mAngleA;    // _54
};