#pragma once

#include "Game/Camera/CameraCharmedVecRegTower.h"

class CamTranslatorCharmedVecRegTower : public CamTranslatorDummy {
public:
	inline CamTranslatorCharmedVecRegTower(CameraCharmedVecRegTower *pCamera) : CamTranslatorDummy(pCamera) {

	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};