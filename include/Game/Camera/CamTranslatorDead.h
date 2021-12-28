#pragma once

#include "Game/Camera/CameraDead.h"

class CamTranslatorDead : public CamTranslatorDummy {
public:
	CamTranslatorDead(CameraDead *);

    virtual void setParam(const CameraParamChunk *);
    virtual Camera *getCamera() const;
};