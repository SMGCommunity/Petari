#pragma once

#include "Game/Camera/CameraInwardTower.hpp"

class CamTranslatorInwardTower : public CamTranslatorBase {
public:
    CamTranslatorInwardTower(CameraInwardTower* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    CameraInwardTower* mCamera;  // 0x4
};