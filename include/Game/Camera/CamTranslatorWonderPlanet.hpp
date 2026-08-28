#pragma once

#include "Game/Camera/CameraWonderPlanet.hpp"

class CamTranslatorWonderPlanet : public CamTranslatorBase {
public:
    CamTranslatorWonderPlanet(CameraWonderPlanet*);

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    /* 0x4 */ CameraWonderPlanet* mCamera;
};
