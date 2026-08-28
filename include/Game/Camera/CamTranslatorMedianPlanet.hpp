#pragma once

#include "Game/Camera/CameraMedianPlanet.hpp"

class CamTranslatorMedianPlanet : public CamTranslatorBase {
public:
    CamTranslatorMedianPlanet(CameraMedianPlanet*);

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    CameraMedianPlanet* mCamera;  // 0x4
};