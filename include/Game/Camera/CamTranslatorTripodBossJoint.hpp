#pragma once

#include "Game/Camera/CameraTripodBossJoint.hpp"

class CamTranslatorTripodBossJoint : public CamTranslatorBase {
public:
    CamTranslatorTripodBossJoint(CameraTripodBossJoint* pCamera) : mCamera(pCamera) {
    }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    /* 0x4 */ CameraTripodBossJoint* mCamera;
};
