#pragma once

#include "Game/Camera/CameraFollow.h"

class CamTranslatorFollow : public CamTranslatorBase {
public:
	CamTranslatorFollow(CameraFollow *);

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;

	CameraFollow *mCamera;	// _4
};