#pragma once

#include "Game/Camera/CameraInnerCylinder.h"

class CamTranslatorInnerCylinder : public CamTranslatorBase {
public:
	inline CamTranslatorInnerCylinder(CameraInnerCylinder *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;

	CameraInnerCylinder *mCamera;	// _4
};