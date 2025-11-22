#pragma once

#include "Game/Camera/CameraCubePlanet.hpp"

class CamTranslatorCubePlanet : public CamTranslatorBase {
public:
    inline CamTranslatorCubePlanet(CameraCubePlanet* pCamera) { mCamera = pCamera; }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraCubePlanet* mCamera;  // 0x4
};