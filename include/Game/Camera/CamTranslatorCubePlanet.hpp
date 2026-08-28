#pragma once

#include "Game/Camera/CameraCubePlanet.hpp"

class CamTranslatorCubePlanet : public CamTranslatorBase {
public:
    CamTranslatorCubePlanet(CameraCubePlanet* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    /* 0x4 */ CameraCubePlanet* mCamera;
};
