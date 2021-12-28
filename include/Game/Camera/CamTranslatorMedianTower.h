#pragma once

#include "Game/Camera/CameraMedianTower.h"

class CamTranslatorMedianTower : public CamTranslatorDummy {
public:
	inline CamTranslatorMedianTower(CameraMedianTower *pCamera) : CamTranslatorDummy(pCamera) {

	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};