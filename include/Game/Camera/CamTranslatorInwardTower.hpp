#pragma once

#include "Game/Camera/CameraInwardTower.hpp"

class CamTranslatorInwardTower : public CamTranslatorBase {
public:
    CamTranslatorInwardTower(CameraInwardTower* pCamera) : mCamera(pCamera) {
    }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    /* 0x4 */ CameraInwardTower* mCamera;
};