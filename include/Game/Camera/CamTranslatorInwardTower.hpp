#pragma once

#include "Game/Camera/CameraInwardTower.hpp"

class CamTranslatorInwardTower : public CamTranslatorBase {
public:
    CamTranslatorInwardTower(CameraInwardTower* pCamera) : mCamera(pCamera) {
    }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraInwardTower* mCamera;  // 0x4
};