#pragma once

#include "Game/Camera/CameraTripodPlanet.hpp"

class CamTranslatorTripodPlanet : public CamTranslatorBase {
public:
    CamTranslatorTripodPlanet(CameraTripodPlanet* pCamera) : mCamera(pCamera) {
    }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    /* 0x4 */ CameraTripodPlanet* mCamera;
};
