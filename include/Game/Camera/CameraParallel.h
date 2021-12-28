#pragma once

#include "Game/Camera/Camera.h"

class CameraParallel : public Camera {
public:
    CameraParallel(const char *);
    virtual ~CameraParallel();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorDummy *createTranslator();
    virtual bool isEnableToRoundLeft() const;
    virtual bool isEnableToRoundRight() const;
    virtual bool isEnableToReset() const;

    void calcIdealPose();
    void calcRound();

    f32 _4C;
    f32 _50;
    f32 _54;
    u8 _58;
    u8 _59;
    u8 _5A[2];
    f32 _5C;
    f32 _60;
    f32 _64;
};