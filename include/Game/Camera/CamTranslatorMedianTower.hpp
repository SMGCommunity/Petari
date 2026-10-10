#pragma once

#include "Game/Camera/CameraMedianTower.hpp"

class CamTranslatorMedianTower : public CamTranslatorBase {
public:
    CamTranslatorMedianTower(CameraMedianTower* pCamera) : mCamera(pCamera) {
    }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    /* 0x4 */ CameraMedianTower* mCamera;
};