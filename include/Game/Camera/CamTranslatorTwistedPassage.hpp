#pragma once

#include "Game/Camera/CameraTwistedPassage.hpp"

class CamTranslatorTwistedPassage : public CamTranslatorBase {
public:
    CamTranslatorTwistedPassage(CameraTwistedPassage* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    /* 0x4 */ CameraTwistedPassage* mCamera;
};
