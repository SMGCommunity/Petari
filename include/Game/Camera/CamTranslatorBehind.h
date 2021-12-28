#pragma once

#include "Game/Camera/CameraBehind.h"

class CamTranslatorBehind : public CamTranslatorDummy {
public:
	inline CamTranslatorBehind(CameraBehind *pCamera) : CamTranslatorDummy(pCamera) {

	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};