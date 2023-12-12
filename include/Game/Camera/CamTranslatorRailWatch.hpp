#pragma once

#include "Game/Camera/CameraRailWatch.h"

class CamTranslatorRailWatch : public CamTranslatorBase {
public:
	inline CamTranslatorRailWatch(CameraRailWatch *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;

	CameraRailWatch *mCamera;	// _4
};