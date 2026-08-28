#pragma once

#include "Game/Camera/CameraCharmedFix.hpp"

class CamTranslatorCharmedFix : public CamTranslatorBase {
public:
    CamTranslatorCharmedFix(CameraCharmedFix* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    /* 0x4 */ CameraCharmedFix* mCamera;
};
