#pragma once

#include "Game/Camera/CameraDPD.hpp"

class CamTranslatorDPD : public CamTranslatorBase {
public:
    inline CamTranslatorDPD(CameraDPD* pCamera) { mCamera = pCamera; }

    void setParam(const CameraParamChunk*);
    Camera* getCamera() const;

    CameraDPD* mCamera;  // 0x4
};