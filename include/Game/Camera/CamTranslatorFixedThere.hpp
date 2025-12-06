#pragma once

#include "Game/Camera/CameraFixedThere.hpp"

class CamTranslatorFixedThere : public CamTranslatorBase {
public:
    inline CamTranslatorFixedThere(CameraFixedThere* pCamera) { mCamera = pCamera; }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraFixedThere* mCamera;  // 0x4
};