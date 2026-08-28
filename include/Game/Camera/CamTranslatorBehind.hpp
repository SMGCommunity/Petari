#pragma once

#include "Game/Camera/CameraBehind.hpp"

class CamTranslatorBehind : public CamTranslatorBase {
public:
    CamTranslatorBehind(CameraBehind* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    /* 0x4 */ CameraBehind* mCamera;
};
