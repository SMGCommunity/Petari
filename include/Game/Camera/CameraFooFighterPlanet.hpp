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

    f32 mAxisX;   // 0x4C
    f32 mAxisY;   // 0x50
    f32 mAngleA;  // 0x54
};