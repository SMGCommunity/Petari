#pragma once

#include "Game/Camera/CameraTower.h"

class CamTranslatorTower : public CamTranslatorBase {
public:
	inline CamTranslatorTower(CameraTower *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;

	CameraTower *mCamera;	// _4
};