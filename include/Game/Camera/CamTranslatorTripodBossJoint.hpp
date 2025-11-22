#pragma once

#include "Game/Camera/CameraTripodBossJoint.hpp"

class CamTranslatorTripodBossJoint : public CamTranslatorBase {
public:
    inline CamTranslatorTripodBossJoint(CameraTripodBossJoint* pCamera) { mCamera = pCamera; }

    virtual void setParam(const CameraParamChunk*);
    virtual Camera* getCamera() const;

    CameraTripodBossJoint* mCamera;  // 0x4
};