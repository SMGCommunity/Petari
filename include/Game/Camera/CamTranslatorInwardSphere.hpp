#pragma once

#include "Game/Camera/CameraInwardSphere.hpp"

class CamTranslatorInwardSphere : public CamTranslatorBase {
public:
    CamTranslatorInwardSphere(CameraInwardSphere* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    CameraInwardSphere* mCamera;  // 0x4
};