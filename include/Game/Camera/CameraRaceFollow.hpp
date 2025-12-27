#pragma once

#include "Game/Camera/Camera.hpp"

class CameraRaceFollow : public Camera {
public:
    CameraRaceFollow(const char* pName = "CameraRaceFollow");
    virtual ~CameraRaceFollow();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    void goRound();

    f32 mWPointX;  // 0x4C
    f32 mWPointY;  // 0x50
    f32 mAngleA;   // 0x54
    bool _58;
    u8 _59[3];
    f32 mWPointZ;  // 0x5C
};