#pragma once

#include "Game/Camera/CameraMedianTower.hpp"

class CamTranslatorMedianTower : public CamTranslatorBase {
public:
    CamTranslatorMedianTower(CameraMedianTower* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    CameraMedianTower* mCamera;  // 0x4
};