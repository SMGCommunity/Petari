#pragma once

#include "Game/Camera/CameraWaterPlanetBoss.hpp"

class CamTranslatorWaterPlanetBoss : public CamTranslatorBase {
public:
    CamTranslatorWaterPlanetBoss(CameraWaterPlanetBoss*);

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    /* 0x4 */ CameraWaterPlanetBoss* mCamera;
};
