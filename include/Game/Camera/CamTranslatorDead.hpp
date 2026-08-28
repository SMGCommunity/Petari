#pragma once

#include "Game/Camera/CameraDead.hpp"

class CamTranslatorDead : public CamTranslatorBase {
public:
    CamTranslatorDead(CameraDead*);

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    /* 0x4 */ CameraDead* mCamera;
};
