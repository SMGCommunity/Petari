#pragma once

#include "Game/Camera/CameraCharmedVecRegTower.hpp"

class CamTranslatorCharmedVecRegTower : public CamTranslatorBase {
public:
    CamTranslatorCharmedVecRegTower(CameraCharmedVecRegTower* pCamera) : mCamera(pCamera) {
    }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    /* 0x4 */ CameraCharmedVecRegTower* mCamera;
};
