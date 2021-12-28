#pragma once

#include "Game/Camera/CameraWaterFollow.h"

class CamTranslatorWaterFollow : public CamTranslatorDummy {
public:
	CamTranslatorWaterFollow(CameraWaterFollow *);

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};