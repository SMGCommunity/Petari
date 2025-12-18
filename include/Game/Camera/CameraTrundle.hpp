#pragma once

#include "Game/Camera/Camera.hpp"

class CameraTrundle : public Camera {
public:
    CameraTrundle(const char* pName = "トランドルカメラ");
    virtual ~CameraTrundle();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    TVec3f mWPoint;  // 0x4C
    TVec3f mAxis;    // 0x58
    f32 mDist;       // 0x64
    f32 mAngleA;     // 0x68
    f32 mAngleB;     // 0x6C
    f32 mUpX;        // 0x70
};