#pragma once

#include "Game/Camera/CameraTower.h"

class CamTranslatorTower : public CamTranslatorDummy {
public:
	inline CamTranslatorTower(CameraTower *pCamera) : CamTranslatorDummy(pCamera) {

	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};