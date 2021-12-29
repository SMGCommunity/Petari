#pragma once

#include "Game/Camera/CameraWaterPlanet.h"

class CamTranslatorWaterPlanet : public CamTranslatorBase {
public:
	CamTranslatorWaterPlanet(CameraWaterPlanet *);

    virtual void setParam(const CameraParamChunk *);
    virtual Camera *getCamera() const;

    CameraWaterPlanet *mCamera; // _4
};