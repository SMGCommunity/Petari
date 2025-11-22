#pragma once

#include "Game/Camera/CameraFrontAndBack.hpp"

class CamTranslatorFrontAndBack : public CamTranslatorBase {
public:
    CamTranslatorFrontAndBack(CameraFrontAndBack* pCamera) { mCamera = pCamera; }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraFrontAndBack* mCamera;  // 0x4
};