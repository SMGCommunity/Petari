#pragma once

#include "Game/Camera/CameraRailFollow.h"

class CamTranslatorRailFollow : public CamTranslatorDummy {
public:
	inline CamTranslatorRailFollow(CameraRailFollow *pCamera) : CamTranslatorDummy(pCamera) {

	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};