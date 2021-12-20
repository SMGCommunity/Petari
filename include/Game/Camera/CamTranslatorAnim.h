#pragma once

#include "Game/Camera/Camera.h"
#include "Game/Camera/CameraAnim.h"

class CameraAnim;
class CameraParamChunk;

class CamTranslatorAnim : public CamTranslatorDummy {
public:
    inline CamTranslatorAnim(CameraAnim *pCamera) : CamTranslatorDummy(pCamera) {

    }

    virtual void setParam(const CameraParamChunk *);
    virtual Camera *getCamera() const;
};