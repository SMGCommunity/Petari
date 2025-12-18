#pragma once

#include "Game/Camera/Camera.hpp"

class CameraTripodBossJoint : public Camera {
public:
    CameraTripodBossJoint(const char* pName = "三脚ボス関節カメラ");
    virtual ~CameraTripodBossJoint();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual bool isEnableToReset() const;
    virtual CamTranslatorBase* createTranslator();

    void calcIdealPose();

    f32 mAngleB;  // 0x4C
    f32 mAngleA;  // 0x50
    f32 mDist;    // 0x54
    s32 _58;
    f32 mAxisX;  // 0x5C
    f32 mAxisY;  // 0x60
    f32 mAxisZ;  // 0x64
    bool _68;
    u8 _69[3];
    f32 _6C;
    f32 _70;
};