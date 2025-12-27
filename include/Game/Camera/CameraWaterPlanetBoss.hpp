#pragma once

#include "Game/Camera/Camera.hpp"

class CameraWaterPlanetBoss : public Camera {
public:
    CameraWaterPlanetBoss(const char* pName = "水中プラネットボス");
    virtual ~CameraWaterPlanetBoss();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual bool isEnableToReset() const;
    virtual CamTranslatorBase* createTranslator();

    f32 mAxisY;  // 0x4C
    f32 mAxisX;  // 0x50
    f32 mDist;   // 0x54
    f32 _58;
    u32 _5C;
    f32 _60;
    f32 _64;
    f32 _68;
    bool _6C;
    u8 _6D[3];
    f32 mWPointX;  // 0x70
    f32 mWPointY;  // 0x74
    f32 mWPointZ;  // 0x78
    f32 mAxisZ;    // 0x7C
    f32 mUpY;      // 0x80
    f32 mUpX;      // 0x84
    f32 mUpZ;      // 0x88
    bool _8C;
    u8 _8D[3];
    u32 _90;
    f32 _94;
    f32 _98;
    f32 _9C;
};