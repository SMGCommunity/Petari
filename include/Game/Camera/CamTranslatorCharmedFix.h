#pragma once

#include "Game/Camera/CameraCharmedFix.h"

class CamTranslatorCharmedFix : public CamTranslatorDummy {
public:
	inline CamTranslatorCharmedFix(CameraCharmedFix *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};