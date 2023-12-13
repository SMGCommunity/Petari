#pragma once

#include "Game/Camera/CameraBlackHole.hpp"

class CamTranslatorBlackHole : public CamTranslatorBase {
public:
	inline CamTranslatorBlackHole(CameraBlackHole *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;

	CameraBlackHole *mCamera;	// _4
};