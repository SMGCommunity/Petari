#pragma once

#include "Game/Camera/CameraFooFighterPlanet.hpp"

class CamTranslatorFooFighterPlanet : public CamTranslatorBase {
public:
    CamTranslatorFooFighterPlanet(CameraFooFighterPlanet*);

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    /* 0x4 */ CameraFooFighterPlanet* mCamera;
};
