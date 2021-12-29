#pragma once

#include "Game/Camera/CameraTwistedPassage.h"

class CamTranslatorTwistedPassage : public CamTranslatorBase {
public:
	inline CamTranslatorTwistedPassage(CameraTwistedPassage *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;

	CameraTwistedPassage *mCamera;	// _4
};