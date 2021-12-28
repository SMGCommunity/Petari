#pragma once

#include "Game/Camera/CameraFixedThere.h"

class CamTranslatorFixedThere : public CamTranslatorDummy {
public:
	inline CamTranslatorFixedThere(CameraFixedThere *pCamera) : CamTranslatorDummy(pCamera) {

	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};