#pragma once

#include "Game/Camera/CameraWaterFollow.hpp"

class CamTranslatorWaterFollow : public CamTranslatorBase {
public:
    CamTranslatorWaterFollow(CameraWaterFollow*);

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    /* 0x4 */ CameraWaterFollow* mCamera;
};
