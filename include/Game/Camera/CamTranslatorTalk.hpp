#pragma once

#include "Game/Camera/CameraTalk.hpp"

class CamTranslatorTalk : public CamTranslatorBase {
public:
    CamTranslatorTalk(CameraTalk* pCamera) : mCamera(pCamera) {
    }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    /* 0x4 */ CameraTalk* mCamera;
};
