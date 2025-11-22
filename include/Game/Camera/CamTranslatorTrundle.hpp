#pragma once

#include "Game/Camera/CameraTrundle.hpp"

class CamTranslatorTrundle : public CamTranslatorBase {
public:
    inline CamTranslatorTrundle(CameraTrundle* pCamera) { mCamera = pCamera; }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraTrundle* mCamera;  // 0x4
};