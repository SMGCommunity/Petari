#pragma once

#include "Game/Camera/CameraCharmedFix.hpp"

class CamTranslatorCharmedFix : public CamTranslatorBase {
public:
    CamTranslatorCharmedFix(CameraCharmedFix* pCamera) : mCamera(pCamera) {
    }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    /* 0x4 */ CameraCharmedFix* mCamera;
};
