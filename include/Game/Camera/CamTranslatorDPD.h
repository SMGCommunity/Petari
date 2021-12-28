#pragma once

#include "Game/Camera/CameraDPD.h"

class CamTranslatorDPD : public CamTranslatorDummy {
public:
	inline CamTranslatorDPD(CameraDPD *pCamera) : CamTranslatorDummy(pCamera) {

	}

	void setParam(const CameraParamChunk *);
	Camera *getCamera() const;
};