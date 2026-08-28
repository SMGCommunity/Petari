#pragma once

#include "Game/Camera/CameraTrundle.hpp"

class CamTranslatorTrundle : public CamTranslatorBase {
public:
    CamTranslatorTrundle(CameraTrundle* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    /* 0x4 */ CameraTrundle* mCamera;
};
