#pragma once

#include "Game/Camera/CameraCharmedTripodBoss.hpp"

class CamTranslatorCharmedTripodBoss : public CamTranslatorBase {
public:
    CamTranslatorCharmedTripodBoss(CameraCharmedTripodBoss* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    /* 0x4 */ CameraCharmedTripodBoss* mCamera;
};
