#pragma once

#include "Game/Camera/CameraFixedPoint.hpp"

class CamTranslatorFixedPoint : public CamTranslatorBase {
public:
    inline CamTranslatorFixedPoint(CameraFixedPoint* pCamera) { mCamera = pCamera; }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraFixedPoint* mCamera;  // 0x4
};