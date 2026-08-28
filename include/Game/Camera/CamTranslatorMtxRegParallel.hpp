#pragma once

#include "Game/Camera/CameraMtxRegParallel.hpp"

class CamTranslatorMtxRegParallel : public CamTranslatorBase {
public:
    CamTranslatorMtxRegParallel(CameraMtxRegParallel* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    CameraMtxRegParallel* mCamera;  // 0x4
};