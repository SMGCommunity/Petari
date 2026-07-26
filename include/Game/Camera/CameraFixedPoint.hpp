#pragma once

#include "Game/Camera/Camera.hpp"

class CameraFixedPoint : public Camera {
public:
    CameraFixedPoint(const char* pName = "定点カメラ");

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void setParam(const TVec3f&, u32);

    /* 0x4C */ TVec3f mPos;
    /* 0x58 */ u32 _58;
};
