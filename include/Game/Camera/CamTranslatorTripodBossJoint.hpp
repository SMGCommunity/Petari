#pragma once

#include "Game/Camera/CameraTripodBossJoint.hpp"

class CamTranslatorTripodBossJoint : public CamTranslatorBase {
public:
    CamTranslatorTripodBossJoint(CameraTripodBossJoint* pCamera) : mCamera(pCamera) {
    }

    /* 0x08 */ virtual void setParam(const CameraParamChunk*);
    /* 0x0C */ virtual Camera* getCamera() const;

    /* 0x4 */ CameraTripodBossJoint* mCamera;
};
