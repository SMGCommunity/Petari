#pragma once

#include "Game/Camera/CameraParallel.hpp"

class CamTranslatorParallel : public CamTranslatorBase {
public:
    CamTranslatorParallel(CameraParallel* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    CameraParallel* mCamera;  // 0x4
};