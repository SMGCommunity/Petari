#pragma once

#include "Game/Camera/Camera.hpp"

class CameraBlackHole : public Camera {
public:
    CameraBlackHole(const char* pName = "ブラックホールカメラ");

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    /* 0x4C */ f32 mFovy;
    /* 0x50 */ f32 mRoll;
    /* 0x54 */ TVec3f mWPoint;
    /* 0x60 */ TVec3f mBasePos;
};
