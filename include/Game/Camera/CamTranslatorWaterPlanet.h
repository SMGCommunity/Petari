#pragma once

#include "Game/Camera/CameraWaterPlanet.h"

class CamTranslatorWaterPlanet : public CamTranslatorDummy {
public:
	CamTranslatorWaterPlanet(CameraWaterPlanet *);

    virtual void setParam(const CameraParamChunk *);
    virtual Camera *getCamera() const;
};