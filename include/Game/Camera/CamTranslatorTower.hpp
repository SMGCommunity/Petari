#pragma once

#include "Game/Camera/CameraTower.hpp"

class CamTranslatorTower : public CamTranslatorBase {
public:
    CamTranslatorTower(CameraTower* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    /* 0x4 */ CameraTower* mCamera;
};
