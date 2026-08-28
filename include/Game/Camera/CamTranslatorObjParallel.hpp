#pragma once

#include "Game/Camera/CameraObjParallel.hpp"

class CamTranslatorObjParallel : public CamTranslatorBase {
public:
    CamTranslatorObjParallel(CameraObjParallel* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    CameraObjParallel* mCamera;  // 0x4
};