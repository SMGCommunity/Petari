#pragma once

#include "Game/Camera/CameraWaterFollow.h"

class CamTranslatorWaterFollow : public CamTranslatorBase {
public:
	CamTranslatorWaterFollow(CameraWaterFollow *);

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;

	CameraWaterFollow *mCamera;	// _4
};