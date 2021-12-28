#pragma once

#include "Game/Camera/CameraCharmedFix.h"

class CamTranslatorCharmedFix : public CamTranslatorDummy {
public:
	inline CamTranslatorCharmedFix(CameraCharmedFix *pCamera) : CamTranslatorDummy(pCamera) {

	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};