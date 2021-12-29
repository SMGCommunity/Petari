#pragma once

#include "Game/Camera/CameraCharmedTripodBoss.h"

class CamTranslatorCharmedTripodBoss : public CamTranslatorDummy {
public:
	inline CamTranslatorCharmedTripodBoss(CameraCharmedTripodBoss *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};