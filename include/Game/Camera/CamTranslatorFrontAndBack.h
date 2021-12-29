#pragma once

#include "Game/Camera/CameraFrontAndBack.h"

class CamTranslatorFrontAndBack : public CamTranslatorDummy {
public:
	CamTranslatorFrontAndBack(CameraFrontAndBack *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};