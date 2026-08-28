#pragma once

#include "Game/Camera/CameraFix.hpp"

class CamTranslatorFix : public CamTranslatorBase {
public:
    CamTranslatorFix(CameraFix* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    /* 0x4 */ CameraFix* mCamera;
};
