#pragma once

#include "Game/Camera/CameraWonderPlanet.h"

class CamTranslatorWonderPlanet : public CamTranslatorDummy {
public:
	CamTranslatorWonderPlanet(CameraWonderPlanet *);

    virtual void setParam(const CameraParamChunk *);
    virtual Camera *getCamera() const;
};