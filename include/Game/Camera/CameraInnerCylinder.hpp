#pragma once

#include "Game/Camera/Camera.hpp"

class CameraInnerCylinder : public Camera {
public:
    CameraInnerCylinder(const char* pName = "CameraInnerCylinder");
    virtual ~CameraInnerCylinder();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    TVec3f mWPoint;  // 0x4C
    TVec3f mAxis;    // 0x58
    f32 mAngleA;     // 0x64
    f32 mAngleB;     // 0x68
    f32 mDist;       // 0x6C
    f32 mUpX;        // 0x70
    f32 _74;
    f32 _78;
    f32 _7C;
    bool _80;
    u8 _81[3];
};