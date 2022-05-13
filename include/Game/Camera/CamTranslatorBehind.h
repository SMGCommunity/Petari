#pragma once

#include "Game/Camera/CameraBehind.h"

class CamTranslatorBehind : public CamTranslatorBase {
public:
	inline CamTranslatorBehind(CameraBehind *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;

	CameraBehind *mCamera;	// _4
};