#pragma once

#include "Game/Camera/Camera.hpp"

class CameraDead : public Camera {
public:
    CameraDead(const char* pName = "通常死亡カメラ");

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual bool isInterpolationOff() const {
        return true;
    }
    virtual CamTranslatorBase* createTranslator();

    /* 0x4C */ f32 mFovy;
    /* 0x50 */ f32 mDist;
    /* 0x54 */ s32 mDeadTime;
    /* 0x58 */ s32 _58;  // TODO: is this a state or a bool?
    /* 0x5C */ s32 mDeadFrame;
};
