#pragma once

#include "Game/Camera/CameraTripodBossJoint.h"

class CamTranslatorTripodBossJoint : public CamTranslatorBase {
public:
	inline CamTranslatorTripodBossJoint(CameraTripodBossJoint *pCamera) {
		mCamera = pCamera;
	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;

	CameraTripodBossJoint *mCamera;	// _4
};