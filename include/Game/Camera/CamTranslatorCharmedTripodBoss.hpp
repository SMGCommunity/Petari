#pragma once

#include "Game/Camera/CameraCharmedTripodBoss.hpp"

class CamTranslatorCharmedTripodBoss : public CamTranslatorBase {
public:
    inline CamTranslatorCharmedTripodBoss(CameraCharmedTripodBoss* pCamera) { mCamera = pCamera; }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraCharmedTripodBoss* mCamera;  // 0x4
};