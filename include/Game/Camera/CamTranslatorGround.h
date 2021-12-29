#pragma once

#include "Game/Camera/CameraGround.h"

class CamTranslatorGround : public CamTranslatorDummy {
public:
	inline CamTranslatorGround(CameraGround *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};