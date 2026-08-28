#pragma once

#include "Game/Camera/CameraRailWatch.hpp"

class CamTranslatorRailWatch : public CamTranslatorBase {
public:
    CamTranslatorRailWatch(CameraRailWatch* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    CameraRailWatch* mCamera;  // 0x4
};