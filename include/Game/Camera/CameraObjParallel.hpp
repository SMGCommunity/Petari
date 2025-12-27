#pragma once

#include "Game/Camera/Camera.hpp"

class CameraObjParallel : public Camera {
public:
    CameraObjParallel(const char* pName = "オブジェ並行カメラ");
    virtual ~CameraObjParallel();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    f32 mAngleA;  // 0x4C
    f32 mAngleB;  // 0x50
    f32 mDist;    // 0x54
};