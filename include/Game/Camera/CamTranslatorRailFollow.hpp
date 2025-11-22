#pragma once

#include "Game/Camera/CameraRailFollow.hpp"

class CamTranslatorRailFollow : public CamTranslatorBase {
public:
    inline CamTranslatorRailFollow(CameraRailFollow* pCamera) { mCamera = pCamera; }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraRailFollow* mCamera;  // 0x4
};