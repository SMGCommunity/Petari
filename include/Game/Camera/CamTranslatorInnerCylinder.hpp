#pragma once

#include "Game/Camera/CameraInnerCylinder.hpp"

class CamTranslatorInnerCylinder : public CamTranslatorBase {
public:
    CamTranslatorInnerCylinder(CameraInnerCylinder* pCamera) : mCamera(pCamera) {
    }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    /* 0x4 */ CameraInnerCylinder* mCamera;
};