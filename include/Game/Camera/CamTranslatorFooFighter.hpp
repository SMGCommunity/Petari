#pragma once

#include "Game/Camera/CameraFooFighter.hpp"

class CamTranslatorFooFighter : public CamTranslatorBase {
public:
    CamTranslatorFooFighter(CameraFooFighter*);

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    /* 0x4 */ CameraFooFighter* mCamera;
};
