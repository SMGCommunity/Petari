#pragma once

#include "Game/Camera/CameraSlide.hpp"

class CamTranslatorSlide : public CamTranslatorBase {
public:
    CamTranslatorSlide(CameraSlide* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    /* 0x4 */ CameraSlide* mCamera;
};
