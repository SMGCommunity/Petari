#pragma once

#include "Game/Camera/CameraMtxRegParallel.hpp"

class CamTranslatorMtxRegParallel : public CamTranslatorBase {
public:
    CamTranslatorMtxRegParallel(CameraMtxRegParallel* pCamera) : mCamera(pCamera) {
    }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraMtxRegParallel* mCamera;  // 0x4
};