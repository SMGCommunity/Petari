#pragma once

#include "Game/Camera/CameraRailDemo.h"

class CamTranslatorRailDemo : public CamTranslatorDummy {
public:
	inline CamTranslatorRailDemo(CameraRailDemo *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};