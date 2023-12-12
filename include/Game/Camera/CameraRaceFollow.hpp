#pragma once

#include "Game/Camera/Camera.h"

class CameraRaceFollow : public Camera {
public:
    CameraRaceFollow(const char *);
    virtual ~CameraRaceFollow();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual CamTranslatorBase *createTranslator();

    void goRound();

    f32 mWPointX;   // _4C
    f32 mWPointY;   // _50
    f32 mAngleA;    // _54
    bool _58;
    u8 _59[3];
    f32 mWPointZ;   // _5C
};