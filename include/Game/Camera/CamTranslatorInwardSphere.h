#pragma once

#include "Game/Camera/CameraInwardSphere.h"

class CamTranslatorInwardSphere : public CamTranslatorDummy {
public:
	inline CamTranslatorInwardSphere(CameraInwardSphere *pCamera) : CamTranslatorDummy(pCamera) {

	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};