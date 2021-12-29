#pragma once

#include "Game/Camera/CameraTripodBoss.h"

class CamTranslatorTripodBoss : public CamTranslatorDummy {
public:
	inline CamTranslatorTripodBoss(CameraTripodBoss *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};