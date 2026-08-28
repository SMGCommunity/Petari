#pragma once

#include "Game/Camera/CameraRailDemo.hpp"

class CamTranslatorRailDemo : public CamTranslatorBase {
public:
    CamTranslatorRailDemo(CameraRailDemo* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    CameraRailDemo* mCamera;  // 0x4
};