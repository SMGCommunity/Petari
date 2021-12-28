#pragma once

#include "Game/Camera/CameraTalk.h"

class CamTranslatorTalk : public CamTranslatorDummy {
public:
	inline CamTranslatorTalk(CameraTalk *pCamera) : CamTranslatorDummy(pCamera) {

	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};