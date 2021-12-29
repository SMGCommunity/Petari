#pragma once

#include "Game/Camera/CameraTrundle.h"

class CamTranslatorTrundle : public CamTranslatorBase {
public:
	inline CamTranslatorTrundle(CameraTrundle *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;

	CameraTrundle *mCamera;	// _4
};