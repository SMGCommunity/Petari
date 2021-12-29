#pragma once

#include "Game/Camera/CameraFooFighterPlanet.h"

class CamTranslatorFooFighterPlanet : public CamTranslatorBase {
public:
	CamTranslatorFooFighterPlanet(CameraFooFighterPlanet *);

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;

	CameraFooFighterPlanet *mCamera;	// _4
};