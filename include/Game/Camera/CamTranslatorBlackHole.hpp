#pragma once

#include "Game/Camera/CameraBlackHole.hpp"

class CamTranslatorBlackHole : public CamTranslatorBase {
public:
    CamTranslatorBlackHole(CameraBlackHole* pCamera) : mCamera(pCamera) {
    }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    /* 0x4 */ CameraBlackHole* mCamera;
};
