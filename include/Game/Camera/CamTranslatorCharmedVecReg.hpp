#pragma once

#include "Game/Camera/CameraCharmedVecReg.hpp"

class CamTranslatorCharmedVecReg : public CamTranslatorBase {
public:
    CamTranslatorCharmedVecReg(CameraCharmedVecReg* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    /* 0x4 */ CameraCharmedVecReg* mCamera;
};
