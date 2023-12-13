#include "Game/Camera/CameraTripodBossJoint.hpp"
#include "Game/Camera/CamTranslatorTripodBossJoint.hpp"

CameraTripodBossJoint::CameraTripodBossJoint(const char *pName) : Camera(pName) {
    mAngleB = 0.0f;
    mAngleA = 0.0f;
    mDist = 3000.0f;
    _68 = false;
    _6C = 0.0f;
    _70 = 0.0f;
}

CameraTripodBossJoint::~CameraTripodBossJoint() {

}

CamTranslatorBase *CameraTripodBossJoint::createTranslator() {
    return new CamTranslatorTripodBossJoint(this);
}
