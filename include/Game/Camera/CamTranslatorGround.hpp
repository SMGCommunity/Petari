#pragma once

#include "Game/Camera/CameraGround.hpp"

class CamTranslatorGround : public CamTranslatorBase {
public:
    CamTranslatorGround(CameraGround* pCamera) : mCamera(pCamera) {
    }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraGround* mCamera;
};