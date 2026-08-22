#pragma once

#include "Game/Camera/CameraTowerPos.hpp"

class CamTranslatorTowerPos : public CamTranslatorBase {
public:
    CamTranslatorTowerPos(CameraTowerPos* pCamera) : mCamera(pCamera) {
    }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    /* 0x4 */ CameraTowerPos* mCamera;
};
