#pragma once

#include "Game/Camera/Camera.hpp"

class CameraCharmedFix : public Camera {
public:
    CameraCharmedFix(const char* pName = "サンボカメラ");
    virtual ~CameraCharmedFix();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    TVec3f mAxis;    // 0x4C
    TVec3f mUp;      // 0x58
    TVec3f mWPoint;  // 0x64
};