#pragma once

#include "Game/Camera/CameraSlide.hpp"

class CamTranslatorSlide : public CamTranslatorBase {
public:
    inline CamTranslatorSlide(CameraSlide* pCamera) { mCamera = pCamera; }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraSlide* mCamera;  // 0x4
};