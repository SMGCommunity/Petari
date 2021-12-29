#pragma once

#include "Game/Camera/CameraMtxRegParallel.h"

class CamTranslatorMtxRegParallel : public CamTranslatorDummy {
public:
	inline CamTranslatorMtxRegParallel(CameraMtxRegParallel *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};