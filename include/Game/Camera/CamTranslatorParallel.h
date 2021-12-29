#pragma once

#include "Game/Camera/CameraParallel.h"

class CamTranslatorParallel : public CamTranslatorBase {
public:
	inline CamTranslatorParallel(CameraParallel *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;

	CameraParallel *mCamera;	// _4
};