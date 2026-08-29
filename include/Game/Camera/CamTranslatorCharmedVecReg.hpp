#pragma once

#include "Game/Camera/CameraCharmedVecReg.hpp"

class CamTranslatorCharmedVecReg : public CamTranslatorBase {
public:
    CamTranslatorCharmedVecReg(CameraCharmedVecReg* pCamera) : mCamera(pCamera) {
    }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    /* 0x4 */ CameraCharmedVecReg* mCamera;
};
