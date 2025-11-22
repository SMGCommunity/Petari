#pragma once

#include "Game/Camera/CameraWonderPlanet.hpp"

class CamTranslatorWonderPlanet : public CamTranslatorBase {
public:
    CamTranslatorWonderPlanet(CameraWonderPlanet*);

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraWonderPlanet* mCamera;  // 0x4
};