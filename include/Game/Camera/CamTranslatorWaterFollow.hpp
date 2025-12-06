#pragma once

#include "Game/Camera/CameraWaterFollow.hpp"

class CamTranslatorWaterFollow : public CamTranslatorBase {
public:
    CamTranslatorWaterFollow(CameraWaterFollow*);

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraWaterFollow* mCamera;  // 0x4
};