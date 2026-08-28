#pragma once

#include "Game/Camera/CameraRaceFollow.hpp"

class CamTranslatorRaceFollow : public CamTranslatorBase {
public:
    CamTranslatorRaceFollow(CameraRaceFollow* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    CameraRaceFollow* mCamera;  // 0x4
};