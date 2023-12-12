#pragma once

#include "Game/Camera/CameraCharmedFix.h"

class CamTranslatorCharmedFix : public CamTranslatorBase {
public:
	inline CamTranslatorCharmedFix(CameraCharmedFix *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;

	CameraCharmedFix *mCamera;	// _4
};