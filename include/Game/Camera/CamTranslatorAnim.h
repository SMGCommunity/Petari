#pragma once

#include "Game/Camera/CameraAnim.h"

class CameraParamChunk;

class CamTranslatorAnim : public CamTranslatorDummy {
public:
    inline CamTranslatorAnim(CameraAnim *pCamera) : CamTranslatorDummy(pCamera) {

    }

    virtual void setParam(const CameraParamChunk *);
    virtual Camera *getCamera() const;

    u32 getAnimFrame(const CameraParamChunk *) const;
};