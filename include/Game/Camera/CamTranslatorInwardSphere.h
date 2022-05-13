#pragma once

#include "Game/Camera/CameraInwardSphere.h"

class CamTranslatorInwardSphere : public CamTranslatorBase {
public:
	inline CamTranslatorInwardSphere(CameraInwardSphere *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;

	CameraInwardSphere *mCamera;	// _4
};