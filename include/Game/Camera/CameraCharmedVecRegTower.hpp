#pragma once

#include "Game/Camera/Camera.hpp"

class CameraCharmedVecRegTower : public Camera {
public:
    CameraCharmedVecRegTower(const char* pName = "CameraCharmedVecRegTower");

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    /* 0x4C */ TVec3f mWPoint;
    /* 0x58 */ TVec3f mAxis;
    /* 0x64 */ f32 mAngleA;
    /* 0x68 */ f32 mAngleB;
    /* 0x6C */ f32 mDist;
    /* 0x70 */ bool mArrange;
    /* 0x74 */ const char* mString;
};
