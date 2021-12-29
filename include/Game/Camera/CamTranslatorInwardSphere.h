#pragma once

#include "Game/Camera/CameraInwardSphere.h"

class CamTranslatorInwardSphere : public CamTranslatorDummy {
public:
	inline CamTranslatorInwardSphere(CameraInwardSphere *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};