#pragma once

#include "Game/Camera/Camera.h"

class CameraAnim;
class CameraParamChunk;

class CamTranslatorAnim : public CamTranslatorDummy {
public:
    inline CamTranslatorAnim(CameraAnim *);
    
    virtual void setParam(const CameraParamChunk *);
    virtual Camera *getCamera() const;
};