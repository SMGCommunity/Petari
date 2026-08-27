#pragma once

#include "Game/Camera/CameraCubePlanet.hpp"

class CamTranslatorCubePlanet : public CamTranslatorBase {
public:
    CamTranslatorCubePlanet(CameraCubePlanet* pCamera) : mCamera(pCamera) {
    }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    /* 0x4 */ CameraCubePlanet* mCamera;
};
