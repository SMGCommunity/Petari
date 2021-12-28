#pragma once

#include "Game/Camera/CameraWaterPlanetBoss.h"

class CamTranslatorWaterPlanetBoss : public CamTranslatorDummy {
public:
	CamTranslatorWaterPlanetBoss(CameraWaterPlanetBoss *);

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};