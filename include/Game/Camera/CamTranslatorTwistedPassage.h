#pragma once

#include "Game/Camera/CameraTwistedPassage.h"

class CamTranslatorTwistedPassage : public CamTranslatorDummy {
public:
	inline CamTranslatorTwistedPassage(CameraTwistedPassage *pCamera) : CamTranslatorDummy(pCamera) {

	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};