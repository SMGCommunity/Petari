#pragma once

#include "Game/Camera/CameraParallel.hpp"

class CamTranslatorParallel : public CamTranslatorBase {
public:
    inline CamTranslatorParallel(CameraParallel* pCamera) { mCamera = pCamera; }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraParallel* mCamera;  // 0x4
};