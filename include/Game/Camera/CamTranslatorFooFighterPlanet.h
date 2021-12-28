#pragma once

#include "Game/Camera/CameraFooFighterPlanet.h"

class CamTranslatorFooFighterPlanet : public CamTranslatorDummy {
public:
	CamTranslatorFooFighterPlanet(CameraFooFighterPlanet *);

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};