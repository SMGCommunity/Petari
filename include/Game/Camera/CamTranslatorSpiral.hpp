#pragma once

#include "Game/Camera/CameraSpiral.hpp"

class CamTranslatorSpiral : public CamTranslatorBase {
public:
    CamTranslatorSpiral(CameraSpiral* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    /* 0x4 */ CameraSpiral* mCamera;
};
