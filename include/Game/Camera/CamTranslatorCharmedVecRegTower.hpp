#pragma once

#include "Game/Camera/CameraCharmedVecRegTower.hpp"

class CamTranslatorCharmedVecRegTower : public CamTranslatorBase {
public:
    CamTranslatorCharmedVecRegTower(CameraCharmedVecRegTower* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    /* 0x4 */ CameraCharmedVecRegTower* mCamera;
};
