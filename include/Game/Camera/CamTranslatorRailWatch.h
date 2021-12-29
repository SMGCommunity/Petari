#pragma once

#include "Game/Camera/CameraRailWatch.h"

class CamTranslatorRailWatch : public CamTranslatorDummy {
public:
	inline CamTranslatorRailWatch(CameraRailWatch *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};