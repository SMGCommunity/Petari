#pragma once

#include "Game/Camera/CameraAnim.h"

class CameraParamChunk;

class CamTranslatorAnim : public CamTranslatorBase {
public:
    inline CamTranslatorAnim(CameraAnim *pCamera) {
        mCamera = pCamera;
    }

    virtual void setParam(const CameraParamChunk *);
    virtual Camera *getCamera() const;

    u32 getAnimFrame(const CameraParamChunk *) const;

    CameraAnim *mCamera;    // _4
};