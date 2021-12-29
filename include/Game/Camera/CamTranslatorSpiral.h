#pragma once

#include "Game/Camera/CameraSpiral.h"

class CamTranslatorSpiral : public CamTranslatorDummy {
public:
	inline CamTranslatorSpiral(CameraSpiral *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};