#pragma once

#include "Game/Camera/CameraParallel.h"

class CamTranslatorParallel : public CamTranslatorDummy {
public:
	inline CamTranslatorParallel(CameraParallel *pCamera) : CamTranslatorDummy(pCamera) {

	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};