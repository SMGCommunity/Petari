#pragma once

#include "Game/Camera/CameraTripodBoss.hpp"

class CamTranslatorTripodBoss : public CamTranslatorBase {
public:
    CamTranslatorTripodBoss(CameraTripodBoss* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    /* 0x4 */ CameraTripodBoss* mCamera;
};
