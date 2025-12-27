#pragma once

#include "Game/Camera/Camera.hpp"

class CameraSpiral : public Camera {
public:
    CameraSpiral(const char* pName = "螺旋カメラ（デモ用）");
    virtual ~CameraSpiral();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    s32 _4C;
    u32 _50;
    s32 _54;
    s32 _58;
    f32 mWPointY;  // 0x5C
    f32 mAxisY;    // 0x60
    f32 mWPointZ;  // 0x64
    f32 mAxisZ;    // 0x68
    f32 mWPointX;  // 0x6C
    f32 mAxisX;    // 0x70
};