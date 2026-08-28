#pragma once

#include "Game/Camera/CameraFollow.hpp"

class CamTranslatorFollow : public CamTranslatorBase {
public:
    CamTranslatorFollow(CameraFollow*);

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    CameraFollow* mCamera;  // 0x4
};