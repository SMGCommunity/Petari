#pragma once

#include "Game/Camera/CameraDead.hpp"

class CamTranslatorDead : public CamTranslatorBase {
public:
    CamTranslatorDead(CameraDead*);

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    /* 0x4 */ CameraDead* mCamera;
};
