#pragma once

#include "Game/Camera/CameraFollow.h"

class CamTranslatorFollow : public CamTranslatorDummy {
public:
	CamTranslatorFollow(CameraFollow *);

	virtual void setParam(const CameraParamChunk *);
	virtual Camera *getCamera() const;
};