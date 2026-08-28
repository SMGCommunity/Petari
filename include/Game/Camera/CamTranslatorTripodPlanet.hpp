#pragma once

#include "Game/Camera/CameraTripodPlanet.hpp"

class CamTranslatorTripodPlanet : public CamTranslatorBase {
public:
    CamTranslatorTripodPlanet(CameraTripodPlanet* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    /* 0x4 */ CameraTripodPlanet* mCamera;
};
