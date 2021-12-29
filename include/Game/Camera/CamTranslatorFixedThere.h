#pragma once

#include "Game/Camera/CameraFixedThere.h"

class CamTranslatorFixedThere : public CamTranslatorBase {
public:
	inline CamTranslatorFixedThere(CameraFixedThere *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;

	CameraFixedThere *mCamera;	// _4
};