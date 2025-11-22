#pragma once

#include "Game/Camera/CameraCharmedFix.hpp"

class CamTranslatorCharmedFix : public CamTranslatorBase {
public:
    inline CamTranslatorCharmedFix(CameraCharmedFix* pCamera) { mCamera = pCamera; }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraCharmedFix* mCamera;  // 0x4
};