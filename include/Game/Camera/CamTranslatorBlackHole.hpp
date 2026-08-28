#pragma once

#include "Game/Camera/CameraBlackHole.hpp"

class CamTranslatorBlackHole : public CamTranslatorBase {
public:
    CamTranslatorBlackHole(CameraBlackHole* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    /* 0x4 */ CameraBlackHole* mCamera;
};
