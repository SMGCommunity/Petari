#pragma once

#include "Game/Camera/CameraRailFollow.hpp"

class CamTranslatorRailFollow : public CamTranslatorBase {
public:
    CamTranslatorRailFollow(CameraRailFollow* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    CameraRailFollow* mCamera;  // 0x4
};