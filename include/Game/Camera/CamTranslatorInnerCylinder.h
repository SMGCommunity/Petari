#pragma once

#include "Game/Camera/CameraInnerCylinder.h"

class CamTranslatorInnerCylinder : public CamTranslatorDummy {
public:
	inline CamTranslatorInnerCylinder(CameraInnerCylinder *pCamera) : CamTranslatorDummy(pCamera) {

	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};