#pragma once

#include "Game/Camera/CameraSpiral.hpp"

class CamTranslatorSpiral : public CamTranslatorBase {
public:
    CamTranslatorSpiral(CameraSpiral* pCamera) : mCamera(pCamera) {
    }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    /* 0x4 */ CameraSpiral* mCamera;
};
