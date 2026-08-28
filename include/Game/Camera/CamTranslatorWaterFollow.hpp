#pragma once

#include "Game/Camera/CameraWaterFollow.hpp"

class CamTranslatorWaterFollow : public CamTranslatorBase {
public:
    CamTranslatorWaterFollow(CameraWaterFollow*);

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    /* 0x4 */ CameraWaterFollow* mCamera;
};
