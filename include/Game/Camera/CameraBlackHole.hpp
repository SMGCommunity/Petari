#pragma once

#include "Game/Camera/Camera.hpp"

class CameraBlackHole : public Camera {
public:
    CameraBlackHole(const char* pName = "ブラックホールカメラ");
    virtual ~CameraBlackHole();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    f32 _4C;
    f32 _50;
    TVec3f mWPoint;  // 0x54
    TVec3f mAxis;    // 0x60
};