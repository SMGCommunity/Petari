#pragma once

#include "Game/Camera/CameraTowerPos.h"

class CamTranslatorTowerPos : public CamTranslatorBase {
public:
	inline CamTranslatorTowerPos(CameraTowerPos *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;

	CameraTowerPos *mCamera;	// _4
};