#pragma once

#include "Game/Camera/CameraFixedPoint.hpp"

class CamTranslatorFixedPoint : public CamTranslatorBase {
public:
    CamTranslatorFixedPoint(CameraFixedPoint* pCamera) : mCamera(pCamera) {
    }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    /* 0x4 */ CameraFixedPoint* mCamera;
};