#pragma once

#include "Game/Camera/Camera.hpp"

class CameraGround : public Camera {
public:
    CameraGround(const char* pName = "地面カメラ");
    virtual ~CameraGround();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    f32 mAngleA;  // 0x4C
    f32 mAngleB;  // 0x50
    f32 mDist;    // 0x54
    f32 mUpX;     // 0x58
    f32 mUpY;     // 0x5C
    f32 mUpZ;     // 0x60
    f32 _64;
    f32 _68;
    f32 _6C;
    bool _70;
    u8 _71[3];
};