#pragma once

#include "Game/Camera/CameraRaceFollow.h"

class CamTranslatorRaceFollow : public CamTranslatorBase {
public:
	inline CamTranslatorRaceFollow(CameraRaceFollow *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;

	CameraRaceFollow *mCamera;	// _4
};