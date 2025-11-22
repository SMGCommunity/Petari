#pragma once

#include "Game/Camera/CameraBehind.hpp"

class CamTranslatorBehind : public CamTranslatorBase {
public:
    inline CamTranslatorBehind(CameraBehind* pCamera) { mCamera = pCamera; }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraBehind* mCamera;  // 0x4
};