#pragma once

#include "Game/Camera/CameraSlide.hpp"

class CamTranslatorSlide : public CamTranslatorBase {
public:
    CamTranslatorSlide(CameraSlide* pCamera) : mCamera(pCamera) {
    }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    /* 0x4 */ CameraSlide* mCamera;
};
