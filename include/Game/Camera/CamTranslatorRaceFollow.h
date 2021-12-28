#pragma once

#include "Game/Camera/CameraRaceFollow.h"

class CamTranslatorRaceFollow : public CamTranslatorDummy {
public:
	inline CamTranslatorRaceFollow(CameraRaceFollow *pCamera) : CamTranslatorDummy(pCamera) {

	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};