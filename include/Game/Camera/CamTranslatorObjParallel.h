#pragma once

#include "Game/Camera/CameraObjParallel.h"

class CamTranslatorObjParallel : public CamTranslatorBase {
public:
	inline CamTranslatorObjParallel(CameraObjParallel *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;

	CameraObjParallel *mCamera;	// _4
};