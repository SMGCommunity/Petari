#pragma once

#include "Game/Camera/CameraTripodBossJoint.h"

class CamTranslatorTripodBossJoint : public CamTranslatorDummy {
public:
	inline CamTranslatorTripodBossJoint(CameraTripodBossJoint *pCamera) : CamTranslatorDummy(pCamera) {

	}

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};