#pragma once

#include "Game/Camera/CameraMedianPlanet.h"

class CamTranslatorMedianPlanet : public CamTranslatorBase {
public:
	CamTranslatorMedianPlanet(CameraMedianPlanet *);

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;

	CameraMedianPlanet *mCamera;	// _4
};