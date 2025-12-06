#pragma once

#include "Game/Camera/CameraTalk.hpp"

class CamTranslatorTalk : public CamTranslatorBase {
public:
    inline CamTranslatorTalk(CameraTalk* pCamera) { mCamera = pCamera; }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraTalk* mCamera;  // 0x4
};