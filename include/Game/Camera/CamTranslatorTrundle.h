#pragma once

#include "Game/Camera/CameraTrundle.h"

class CamTranslatorTrundle : public CamTranslatorDummy {
public:
	inline CamTranslatorTrundle(CameraTrundle *pCamera) : CamTranslatorDummy(pCamera) {

	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};