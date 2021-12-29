#pragma once

#include "Game/Camera/CameraTripodBossJoint.h"

class CamTranslatorTripodBossJoint : public CamTranslatorDummy {
public:
	inline CamTranslatorTripodBossJoint(CameraTripodBossJoint *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};