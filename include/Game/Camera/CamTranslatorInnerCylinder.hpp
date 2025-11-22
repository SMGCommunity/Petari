#pragma once

#include "Game/Camera/CameraInnerCylinder.hpp"

class CamTranslatorInnerCylinder : public CamTranslatorBase {
public:
    inline CamTranslatorInnerCylinder(CameraInnerCylinder* pCamera) { mCamera = pCamera; }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraInnerCylinder* mCamera;  // 0x4
};