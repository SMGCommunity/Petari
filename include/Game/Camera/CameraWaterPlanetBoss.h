#pragma once

#include "Game/Camera/Camera.h"

class CameraWaterPlanetBoss : public Camera {
public:
    CameraWaterPlanetBoss(const char *);
    virtual ~CameraWaterPlanetBoss();

    virtual void reset();
    virtual CameraTargetObj *calc();
    virtual bool isEnableToReset() const;
    virtual CamTranslatorDummy *createTranslator();

    f32 mAxisY;     // _4C
    f32 mAxisX;     // _50
    f32 mDist;      // _54
    f32 _58;
    u32 _5C;
    f32 _60;
    f32 _64;
    f32 _68;
    bool _6C;
    u8 _6D[3];
    f32 mWPointX;   // _70
    f32 mWPointY;   // _74
    f32 mWPointZ;   // _78
    f32 mAxisZ;     // _7C
    f32 mUpY;       // _80
    f32 mUpX;       // _84
    f32 mUpZ;       // _88
    bool _8C;
    u8 _8D[3];
    u32 _90;
    f32 _94;
    f32 _98;
    f32 _9C;
};