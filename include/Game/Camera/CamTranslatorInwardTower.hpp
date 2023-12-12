#pragma once

#include "Game/Camera/CameraInwardTower.h"

class CamTranslatorInwardTower : public CamTranslatorBase {
public:
	inline CamTranslatorInwardTower(CameraInwardTower *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;

	CameraInwardTower *mCamera;	// _4
};