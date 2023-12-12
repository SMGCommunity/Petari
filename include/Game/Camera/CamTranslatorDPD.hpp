#pragma once

#include "Game/Camera/CameraDPD.h"

class CamTranslatorDPD : public CamTranslatorBase {
public:
	inline CamTranslatorDPD(CameraDPD *pCamera) {
		mCamera = pCamera;
	}

	void setParam(const CameraParamChunk *);
	Camera *getCamera() const;

	CameraDPD *mCamera;	// _4
};