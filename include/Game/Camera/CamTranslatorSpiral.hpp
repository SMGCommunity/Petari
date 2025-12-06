#pragma once

#include "Game/Camera/CameraSpiral.hpp"

class CamTranslatorSpiral : public CamTranslatorBase {
public:
    inline CamTranslatorSpiral(CameraSpiral* pCamera) { mCamera = pCamera; }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraSpiral* mCamera;  // 0x4
};