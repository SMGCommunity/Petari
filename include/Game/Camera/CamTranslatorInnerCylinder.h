#pragma once

#include "Game/Camera/CameraInnerCylinder.h"

class CamTranslatorInnerCylinder : public CamTranslatorDummy {
public:
	inline CamTranslatorInnerCylinder(CameraInnerCylinder *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};