#pragma once

#include "Game/Camera/CameraCharmedVecRegTower.h"

class CamTranslatorCharmedVecRegTower : public CamTranslatorBase {
public:
	inline CamTranslatorCharmedVecRegTower(CameraCharmedVecRegTower *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;

	CameraCharmedVecRegTower *mCamera;	// _4
};