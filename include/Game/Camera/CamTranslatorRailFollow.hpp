#pragma once

#include "Game/Camera/CameraRailFollow.hpp"

class CamTranslatorRailFollow : public CamTranslatorBase {
public:
    CamTranslatorRailFollow(CameraRailFollow* pCamera) : mCamera(pCamera) {
    }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    /* 0x4 */ CameraRailFollow* mCamera;
};