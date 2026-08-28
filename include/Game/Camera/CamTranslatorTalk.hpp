#pragma once

#include "Game/Camera/CameraTalk.hpp"

class CamTranslatorTalk : public CamTranslatorBase {
public:
    CamTranslatorTalk(CameraTalk* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    /* 0x4 */ CameraTalk* mCamera;
};
