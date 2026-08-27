#pragma once

#include "Game/Camera/CameraDPD.hpp"

class CamTranslatorDPD : public CamTranslatorBase {
public:
    CamTranslatorDPD(CameraDPD* pCamera) : mCamera(pCamera) {
    }

    void setParam(const CameraParamChunk*);
    Camera* getCamera() const;

    /* 0x4 */ CameraDPD* mCamera;
};
