#pragma once

#include "Game/Camera/Camera.hpp"

class CameraSubjective : public Camera {
public:
    CameraSubjective(const char* pName = "主観カメラ");

    /* 0x24 */ virtual void reset();
    /* 0x28 */ virtual CameraTargetObj* calc();
    /* 0x30 */ virtual bool isCollisionOff() const {
        return true;
    }
    /* 0x54 */ virtual CamTranslatorBase* createTranslator();

    /* 0x4C */ TVec2f mAngleVel;
    /* 0x54 */ TVec2f mAngle;
    /* 0x5C */ TPos3f mMtx;
};
