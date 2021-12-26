#pragma once

#include "Game/Camera/Camera.h"

class CameraFooFighterPlanet : public Camera {
public:
    CameraFooFighterPlanet(const char *);
    virtual ~CameraFooFighterPlanet();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorDummy *createTranslator();

    void goRoundBehind(TVec3f &, TVec3f &, TVec3f &);

    f32 _4C;
    f32 _50;
    f32 _54;
};