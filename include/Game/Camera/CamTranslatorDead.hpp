#pragma once

#include "Game/Camera/CameraDead.h"

class CamTranslatorDead : public CamTranslatorBase {
public:
	CamTranslatorDead(CameraDead *);

    virtual void setParam(const CameraParamChunk *);
    virtual Camera *getCamera() const;

    CameraDead *mCamera;    // _4
};