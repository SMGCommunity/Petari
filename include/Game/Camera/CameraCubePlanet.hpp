#pragma once

#include "Game/Camera/Camera.hpp"

class CameraCubePlanet : public Camera {
public:
    CameraCubePlanet(const char* pName = "キューブ惑星カメラ");
    virtual ~CameraCubePlanet();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    f32 mDist;    // 0x4C
    f32 mAngleA;  // 0x50
    f32 mAngleB;  // 0x54
    f32 _58;
    f32 _5C;
    f32 _60;
};