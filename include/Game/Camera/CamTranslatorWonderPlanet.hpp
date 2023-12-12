#pragma once

#include "Game/Camera/CameraWonderPlanet.h"

class CamTranslatorWonderPlanet : public CamTranslatorBase {
public:
	CamTranslatorWonderPlanet(CameraWonderPlanet *);

    virtual void setParam(const CameraParamChunk *);
    virtual Camera *getCamera() const;

    CameraWonderPlanet *mCamera;    // _4
};