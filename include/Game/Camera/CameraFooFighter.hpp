#pragma once

#include "Game/Camera/Camera.hpp"

class CameraFooFighter : public Camera {
public:
    CameraFooFighter(const char* pName = "フーファイター");

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    /* 0x4C */ f32 mDistMin;
    /* 0x50 */ f32 mDistMax;
    /* 0x54 */ f32 _54;
    /* 0x58 */ f32 _58;
    /* 0x5C */ s32 mCollideCount;
    /* 0x60 */ TVec3f mTargetLastMoveDir;
};
