#pragma once

#include "Game/Camera/CameraFooFighter.h"

class CamTranslatorFooFighter : public CamTranslatorDummy {
public:
	CamTranslatorFooFighter(CameraFooFighter *);

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};