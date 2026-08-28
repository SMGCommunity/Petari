#pragma once

#include "Game/Camera/CameraFrontAndBack.hpp"

class CamTranslatorFrontAndBack : public CamTranslatorBase {
public:
    CamTranslatorFrontAndBack(CameraFrontAndBack* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    /* 0x4 */ CameraFrontAndBack* mCamera;
};
