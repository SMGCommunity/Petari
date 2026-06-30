#pragma once

#include "Game/Camera/CameraInwardSphere.hpp"

class CamTranslatorInwardSphere : public CamTranslatorBase {
public:
    CamTranslatorInwardSphere(CameraInwardSphere* pCamera) : mCamera(pCamera) {
    }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraInwardSphere* mCamera;  // 0x4
};