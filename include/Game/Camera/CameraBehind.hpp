#pragma once

#include "Game/Camera/Camera.hpp"

class CameraBehind : public Camera {
public:
    CameraBehind(const char* pName = "後方カメラ");
    virtual ~CameraBehind();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void calcPosAndUp();

    f32 mAngleB;  // 0x4C
    f32 mAngleA;  // 0x50
    f32 mDist;    // 0x54
    f32 mAxisX;   // 0x58
};