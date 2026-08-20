#pragma once

#include "Game/Camera/CameraTrundle.hpp"

class CamTranslatorTrundle : public CamTranslatorBase {
public:
    CamTranslatorTrundle(CameraTrundle* pCamera) : mCamera(pCamera) {
    }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    /* 0x4 */ CameraTrundle* mCamera;
};
