#pragma once

#include "Game/Camera/CameraFollow.hpp"

class CamTranslatorFollow : public CamTranslatorBase {
public:
    CamTranslatorFollow(CameraFollow*);

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraFollow* mCamera;  // 0x4
};