#pragma once

#include "Game/Camera/CameraFrontAndBack.h"

class CamTranslatorFrontAndBack : public CamTranslatorDummy {
public:
	CamTranslatorFrontAndBack(CameraFrontAndBack *pCamera) : CamTranslatorDummy(pCamera) {

	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};