#pragma once

#include "Game/Camera/CameraCharmedVecReg.hpp"

class CamTranslatorCharmedVecReg : public CamTranslatorBase {
public:
    CamTranslatorCharmedVecReg(CameraCharmedVecReg* pCamera) : mCamera(pCamera) {
    }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraCharmedVecReg* mCamera;  // 0x4
};