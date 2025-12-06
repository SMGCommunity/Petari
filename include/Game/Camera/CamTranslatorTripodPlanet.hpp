#pragma once

#include "Game/Camera/CameraTripodPlanet.hpp"

class CamTranslatorTripodPlanet : public CamTranslatorBase {
public:
    inline CamTranslatorTripodPlanet(CameraTripodPlanet* pCamera) { mCamera = pCamera; }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraTripodPlanet* mCamera;  // 0x4
};