#pragma once

#include "Game/Camera/CameraRailWatch.hpp"

class CamTranslatorRailWatch : public CamTranslatorBase {
public:
    CamTranslatorRailWatch(CameraRailWatch* pCamera) : mCamera(pCamera) {
    }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    /* 0x4 */ CameraRailWatch* mCamera;
};