#pragma once

#include "Game/Camera/CameraRailWatch.hpp"

class CamTranslatorRailWatch : public CamTranslatorBase {
public:
    inline CamTranslatorRailWatch(CameraRailWatch* pCamera) { mCamera = pCamera; }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraRailWatch* mCamera;  // 0x4
};