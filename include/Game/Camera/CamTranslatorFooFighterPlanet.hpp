#pragma once

#include "Game/Camera/CameraFooFighterPlanet.hpp"

class CamTranslatorFooFighterPlanet : public CamTranslatorBase {
public:
    CamTranslatorFooFighterPlanet(CameraFooFighterPlanet*);

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    /* 0x4 */ CameraFooFighterPlanet* mCamera;
};
