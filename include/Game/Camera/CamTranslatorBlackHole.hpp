#pragma once

#include "Game/Camera/CameraBlackHole.h"

class CamTranslatorBlackHole : public CamTranslatorBase {
public:
	inline CamTranslatorBlackHole(CameraBlackHole *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;

	CameraBlackHole *mCamera;	// _4
};