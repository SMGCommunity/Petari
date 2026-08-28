#pragma once

#include "Game/Camera/CameraWaterPlanet.hpp"

class CamTranslatorWaterPlanet : public CamTranslatorBase {
public:
    CamTranslatorWaterPlanet(CameraWaterPlanet*);

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    /* 0x4 */ CameraWaterPlanet* mCamera;
};
