#pragma once

#include "Game/Camera/CameraFix.h"

class CamTranslatorFix : public CamTranslatorBase {
public:
	inline CamTranslatorFix(CameraFix *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;

	CameraFix *mCamera;	// _4
};