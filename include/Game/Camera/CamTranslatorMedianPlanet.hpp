#pragma once

#include "Game/Camera/CameraMedianPlanet.hpp"

class CamTranslatorMedianPlanet : public CamTranslatorBase {
public:
    CamTranslatorMedianPlanet(CameraMedianPlanet*);

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraMedianPlanet* mCamera;  // 0x4
};