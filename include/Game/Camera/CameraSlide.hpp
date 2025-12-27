#pragma once

#include "Game/Camera/Camera.hpp"

class CameraSlide : public Camera {
public:
    CameraSlide(const char* pName = "スライドカメラ");
    virtual ~CameraSlide();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    TVec3f mAxis;    // 0x4C
    TVec3f mUp;      // 0x58
    TVec3f mWPoint;  // 0x64
    f32 mAngleA;     // 0x70
    f32 mDist;       // 0x74
};