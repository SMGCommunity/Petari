#pragma once

#include "Game/Camera/Camera.hpp"

class CameraCharmedVecRegTower : public Camera {
public:
    CameraCharmedVecRegTower(const char* pName = "CameraCharmedVecRegTower");
    virtual ~CameraCharmedVecRegTower();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    TVec3f mWPoint;  // 0x4C
    TVec3f mAxis;    // 0x58
    f32 mAngleA;     // 0x64
    f32 mAngleB;     // 0x68
    f32 mDist;       // 0x6C
    bool _70;
    u8 _71[3];
    const char* mString;  // 0x74
};