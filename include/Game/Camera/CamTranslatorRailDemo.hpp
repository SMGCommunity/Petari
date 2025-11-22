#pragma once

#include "Game/Camera/CameraRailDemo.hpp"

class CamTranslatorRailDemo : public CamTranslatorBase {
public:
    inline CamTranslatorRailDemo(CameraRailDemo* pCamera) { mCamera = pCamera; }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraRailDemo* mCamera;  // 0x4
};