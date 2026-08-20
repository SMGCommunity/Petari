#pragma once

#include "Game/Camera/CameraFooFighter.hpp"

class CamTranslatorFooFighter : public CamTranslatorBase {
public:
    CamTranslatorFooFighter(CameraFooFighter*);

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    /* 0x4 */ CameraFooFighter* mCamera;
};
