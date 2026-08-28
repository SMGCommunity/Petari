#pragma once

#include "Game/Camera/CameraAnim.hpp"

class CameraParamChunk;

class CamTranslatorAnim : public CamTranslatorBase {
public:
    CamTranslatorAnim(CameraAnim* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    u32 getAnimFrame(const CameraParamChunk*) const;

    CameraAnim* mCamera;  // 0x4
};