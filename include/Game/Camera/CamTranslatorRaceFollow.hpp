#pragma once

#include "Game/Camera/CameraRaceFollow.hpp"

class CamTranslatorRaceFollow : public CamTranslatorBase {
public:
    CamTranslatorRaceFollow(CameraRaceFollow* pCamera) : mCamera(pCamera) {
    }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    /* 0x4 */ CameraRaceFollow* mCamera;
};