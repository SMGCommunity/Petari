#pragma once

#include "Game/Camera/Camera.h"

class CameraParallel : public Camera {
public:
    CameraParallel(const char *);
    virtual ~CameraParallel();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorBase *createTranslator();
    virtual bool isEnableToRoundLeft() const;
    virtual bool isEnableToRoundRight() const;
    virtual bool isEnableToReset() const;

    void calcIdealPose();
    void calcRound();

    f32 mAngleB;    // _4C
    f32 mAngleA;    // _50
    f32 mDist;      // _54
    bool _58;
    bool _59;
    u8 _5A[2];
    f32 _5C;
    f32 _60;
    f32 _64;
};