#pragma once

#include "Game/Camera/Camera.hpp"

class CameraTripodPlanet : public Camera {
public:
    CameraTripodPlanet(const char* pName = "三脚惑星カメラ");
    virtual ~CameraTripodPlanet();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    TVec3f mAxis;    // 0x4C
    TVec3f mWPoint;  // 0x58
    f32 mAngleA;     // 0x64
    f32 mAngleB;     // 0x68
    f32 mDist;       // 0x6C
    u8 _70[4];
};