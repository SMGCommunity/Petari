#pragma once

#include "Game/Camera/CameraMedianPlanet.h"

class CamTranslatorMedianPlanet : public CamTranslatorDummy {
public:
	CamTranslatorMedianPlanet(CameraMedianPlanet *);

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};