#pragma once

#include "Game/Camera/CameraMedianTower.h"

class CamTranslatorMedianTower : public CamTranslatorBase {
public:
	inline CamTranslatorMedianTower(CameraMedianTower *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;

	CameraMedianTower *mCamera;	// _4
};