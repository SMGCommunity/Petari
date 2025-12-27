#pragma once

#include "Game/Camera/Camera.hpp"

class CameraFixedPoint : public Camera {
public:
    CameraFixedPoint(const char* pName = "定点カメラ");
    virtual ~CameraFixedPoint();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void setParam(const TVec3f&, u32);

    f32 _4C;
    f32 _50;
    f32 _54;
    u8 _58[4];
};