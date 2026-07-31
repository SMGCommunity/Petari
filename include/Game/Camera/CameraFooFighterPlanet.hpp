#pragma once

#include "Game/Camera/Camera.hpp"

class CameraFooFighterPlanet : public Camera {
public:
    CameraFooFighterPlanet(const char* pName = "フーファイタープラネットカメラ");
    virtual ~CameraFooFighterPlanet();
    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void goRoundBehind(TVec3f&, TVec3f&, TVec3f&);

    /* 0x4C */ f32 mDistMin;
    /* 0x50 */ f32 mDistMax;
    /* 0x54 */ f32 mPitchMax;
};
