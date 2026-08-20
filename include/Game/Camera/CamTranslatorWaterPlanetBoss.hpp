#pragma once

#include "Game/Camera/CameraWaterPlanetBoss.hpp"

class CamTranslatorWaterPlanetBoss : public CamTranslatorBase {
public:
    CamTranslatorWaterPlanetBoss(CameraWaterPlanetBoss*);

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    /* 0x4 */ CameraWaterPlanetBoss* mCamera;
};
