#pragma once

#include "Game/Camera/CameraWaterPlanet.hpp"

class CamTranslatorWaterPlanet : public CamTranslatorBase {
public:
    CamTranslatorWaterPlanet(CameraWaterPlanet*);

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraWaterPlanet* mCamera;  // 0x4
};