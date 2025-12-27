#pragma once

#include "Game/Camera/Camera.hpp"

class CameraParallel : public Camera {
public:
    CameraParallel(const char* pName = "平行カメラ");
    virtual ~CameraParallel();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();
    virtual bool isEnableToRoundLeft() const;
    virtual bool isEnableToRoundRight() const;
    virtual bool isEnableToReset() const;

    void calcIdealPose();
    void calcRound();

    f32 mAngleB;  // 0x4C
    f32 mAngleA;  // 0x50
    f32 mDist;    // 0x54
    bool _58;
    bool _59;
    u8 _5A[2];
    f32 _5C;
    f32 _60;
    f32 _64;
};