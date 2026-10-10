#pragma once

#include "Game/Camera/CameraInwardSphere.hpp"

class CamTranslatorInwardSphere : public CamTranslatorBase {
public:
    CamTranslatorInwardSphere(CameraInwardSphere* pCamera) : mCamera(pCamera) {
    }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    /* 0x4 */ CameraInwardSphere* mCamera;
};