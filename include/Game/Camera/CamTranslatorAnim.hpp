#pragma once

#include "Game/Camera/CameraAnim.hpp"

class CameraParamChunk;

class CamTranslatorAnim : public CamTranslatorBase {
public:
    inline CamTranslatorAnim(CameraAnim* pCamera) { mCamera = pCamera; }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    u32 getAnimFrame(const CameraParamChunk*) const;

    CameraAnim* mCamera;  // 0x4
};