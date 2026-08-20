#pragma once

#include "Game/Camera/CameraTwistedPassage.hpp"

class CamTranslatorTwistedPassage : public CamTranslatorBase {
public:
    CamTranslatorTwistedPassage(CameraTwistedPassage* pCamera) : mCamera(pCamera) {
    }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    /* 0x4 */ CameraTwistedPassage* mCamera;
};
