#pragma once

#include "Game/Camera/CameraTripodPlanet.h"

class CamTranslatorTripodPlanet : public CamTranslatorDummy {
public:
	inline CamTranslatorTripodPlanet(CameraTripodPlanet *pCamera) : CamTranslatorDummy(pCamera) {

	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};