#pragma once

#include "Game/Camera/Camera.hpp"

class CameraCharmedFix : public Camera {
public:
    CameraCharmedFix(const char* pName = "サンボカメラ");

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    /* 0x4C */ TVec3f mBasePos;
    /* 0x58 */ TVec3f mUp;
    /* 0x64 */ TVec3f mWPoint;
};
