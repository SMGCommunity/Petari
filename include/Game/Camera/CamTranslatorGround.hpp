#pragma once

#include "Game/Camera/CameraGround.hpp"

class CamTranslatorGround : public CamTranslatorBase {
public:
    CamTranslatorGround(CameraGround* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    CameraGround* mCamera;
};