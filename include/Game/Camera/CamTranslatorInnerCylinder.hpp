#pragma once

#include "Game/Camera/CameraInnerCylinder.hpp"

class CamTranslatorInnerCylinder : public CamTranslatorBase {
public:
    CamTranslatorInnerCylinder(CameraInnerCylinder* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    CameraInnerCylinder* mCamera;  // 0x4
};