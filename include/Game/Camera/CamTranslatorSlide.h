#pragma once

#include "Game/Camera/CameraSlide.h"

class CamTranslatorSlide : public CamTranslatorDummy {
public:
	inline CamTranslatorSlide(CameraSlide *pCamera) : CamTranslatorDummy(pCamera) {

	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};