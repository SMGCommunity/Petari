#pragma once

#include "Game/Camera/Camera.hpp"

class CameraWaterPlanet : public Camera {
public:
    CameraWaterPlanet(const char* pName = "水中プラネットカメラ");
    virtual ~CameraWaterPlanet();

    virtual void reset();
    virtual CameraTargetObj* calc();
    virtual CamTranslatorBase* createTranslator();

    f32 mAxisX;   // 0x4C
    f32 mAxisY;   // 0x50
    f32 mAngleA;  // 0x54
};