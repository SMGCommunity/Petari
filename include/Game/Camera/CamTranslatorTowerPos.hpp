#pragma once

#include "Game/Camera/CameraTowerPos.hpp"

class CamTranslatorTowerPos : public CamTranslatorBase {
public:
    CamTranslatorTowerPos(CameraTowerPos* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    /* 0x4 */ CameraTowerPos* mCamera;
};
