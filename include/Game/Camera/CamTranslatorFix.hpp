#pragma once

#include "Game/Camera/CameraFix.hpp"

class CamTranslatorFix : public CamTranslatorBase {
public:
    inline CamTranslatorFix(CameraFix* pCamera) { mCamera = pCamera; }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraFix* mCamera;  // 0x4
};