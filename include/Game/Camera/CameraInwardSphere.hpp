#pragma once

#include "Game/Camera/Camera.hpp"

class CameraInwardSphere : public Camera {
public:
    CameraInwardSphere(const char* pName = "球内部カメラ");
    virtual ~CameraInwardSphere();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void calcGravityCenter();

    f32 mDist;    // 0x4C
    f32 mAngleA;  // 0x50
    f32 mAngleB;  // 0x54
    f32 _58;
    f32 _5C;
    f32 _60;
    f32 _64;
    f32 _68;
    f32 _6C;
    u8 _70[12];
};