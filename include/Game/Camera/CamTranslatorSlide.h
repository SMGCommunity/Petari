#pragma once

#include "Game/Camera/CameraSlide.h"

class CamTranslatorSlide : public CamTranslatorDummy {
public:
	inline CamTranslatorSlide(CameraSlide *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};