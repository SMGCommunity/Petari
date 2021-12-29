#pragma once

#include "Game/Camera/CameraCharmedVecReg.h"

class CamTranslatorCharmedVecReg : public CamTranslatorDummy {
public:
	inline CamTranslatorCharmedVecReg(CameraCharmedVecReg *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};