#pragma once

#include "Game/Camera/CameraSpiral.h"

class CamTranslatorSpiral : public CamTranslatorDummy {
public:
	inline CamTranslatorSpiral(CameraSpiral *pCamera) : CamTranslatorDummy(pCamera) {

	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};