#pragma once

#include "Game/Camera/CameraFixedPoint.h"

class CamTranslatorFixedPoint : public CamTranslatorDummy {
public:
	inline CamTranslatorFixedPoint(CameraFixedPoint *pCamera) : CamTranslatorDummy(pCamera) {

	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};