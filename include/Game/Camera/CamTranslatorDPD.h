#pragma once

#include "Game/Camera/CameraDPD.h"

class CamTranslatorDPD : public CamTranslatorDummy {
public:
	inline CamTranslatorDPD(CameraDPD *pCamera) {
		mCamera = pCamera;
	}

	void setParam(const CameraParamChunk *);
	Camera *getCamera() const;
};