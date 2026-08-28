#pragma once

#include "Game/Camera/CameraFixedThere.hpp"

class CamTranslatorFixedThere : public CamTranslatorBase {
public:
    CamTranslatorFixedThere(CameraFixedThere* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    /* 0x4 */ CameraFixedThere* mCamera;
};
