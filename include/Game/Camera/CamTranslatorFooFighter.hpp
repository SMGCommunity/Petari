#pragma once

#include "Game/Camera/CameraFooFighter.h"

class CamTranslatorFooFighter : public CamTranslatorBase {
public:
	CamTranslatorFooFighter(CameraFooFighter *);

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;

	CameraFooFighter *mCamera;	// _4
};