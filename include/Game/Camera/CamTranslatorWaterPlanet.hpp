#pragma once

#include "Game/Camera/CameraWaterPlanet.hpp"

class CamTranslatorWaterPlanet : public CamTranslatorBase {
public:
    CamTranslatorWaterPlanet(CameraWaterPlanet*);

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    /* 0x4 */ CameraWaterPlanet* mCamera;
};
