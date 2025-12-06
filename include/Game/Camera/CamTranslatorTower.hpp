#pragma once

#include "Game/Camera/CameraTower.hpp"

class CamTranslatorTower : public CamTranslatorBase {
public:
    inline CamTranslatorTower(CameraTower* pCamera) { mCamera = pCamera; }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraTower* mCamera;  // 0x4
};