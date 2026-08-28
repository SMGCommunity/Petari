#pragma once

#include "Game/Camera/CameraFixedPoint.hpp"

class CamTranslatorFixedPoint : public CamTranslatorBase {
public:
    CamTranslatorFixedPoint(CameraFixedPoint* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    CameraFixedPoint* mCamera;  // 0x4
};