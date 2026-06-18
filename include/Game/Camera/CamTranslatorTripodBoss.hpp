#pragma once

#include "Game/Camera/CameraTripodBoss.hpp"

class CamTranslatorTripodBoss : public CamTranslatorBase {
public:
    CamTranslatorTripodBoss(CameraTripodBoss* pCamera) : mCamera(pCamera) {
    }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraTripodBoss* mCamera;  // 0x4
};