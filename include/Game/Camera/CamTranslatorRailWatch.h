#pragma once

#include "Game/Camera/CameraRailWatch.h"

class CamTranslatorRailWatch : public CamTranslatorDummy {
public:
	inline CamTranslatorRailWatch(CameraRailWatch *pCamera) : CamTranslatorDummy(pCamera) {

	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};