#pragma once

#include "Game/Camera/CameraWaterPlanetBoss.h"

class CamTranslatorWaterPlanetBoss : public CamTranslatorBase {
public:
	CamTranslatorWaterPlanetBoss(CameraWaterPlanetBoss *);

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;

	CameraWaterPlanetBoss *mCamera;	// _4
};