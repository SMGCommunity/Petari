#pragma once

#include "Game/Camera/CameraMedianPlanet.hpp"

class CamTranslatorMedianPlanet : public CamTranslatorBase {
public:
    CamTranslatorMedianPlanet(CameraMedianPlanet*);

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    /* 0x4 */ CameraMedianPlanet* mCamera;
};