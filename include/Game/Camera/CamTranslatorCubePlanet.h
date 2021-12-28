#pragma once

#include "Game/Camera/CameraCubePlanet.h"

class CamTranslatorCubePlanet : public CamTranslatorDummy {
public:
	inline CamTranslatorCubePlanet(CameraCubePlanet *pCamera) : CamTranslatorDummy(pCamera) {

	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};