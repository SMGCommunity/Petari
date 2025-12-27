#pragma once

#include "Game/Camera/Camera.hpp"

class CameraInwardTower : public Camera {
public:
    CameraInwardTower(const char* pName = "塔内部カメラ");
    virtual ~CameraInwardTower();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual bool isEnableToReset() const;
    virtual CamTranslatorBase* createTranslator();

    void calcIdealPose();

    TVec3f mWPoint;  // 0x4C
    TVec3f mAxis;    // 0x58
    f32 mAngleB;     // 0x64
    f32 mAngleA;     // 0x68
    f32 mDist;       // 0x6C
    f32 _70;
    f32 _74;
    f32 _78;
    f32 _7C;
    u8 _80;
    u8 _81[3];
};