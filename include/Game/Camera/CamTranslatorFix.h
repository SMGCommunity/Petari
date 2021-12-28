#pragma once

#include "Game/Camera/CameraFix.h"

class CamTranslatorFix : public CamTranslatorDummy {
public:
	inline CamTranslatorFix(CameraFix *pCamera) : CamTranslatorDummy(pCamera) {

	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};