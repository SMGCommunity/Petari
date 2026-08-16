#include "Game/Camera/CameraTripodBoss.hpp"
#include "Game/Camera/CamTranslatorTripodBoss.hpp"
#include "Game/Camera/CameraLocalUtil.hpp"

void CameraTripodBoss_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

CameraTripodBoss::~CameraTripodBoss() {
}

CameraTripodBoss::CameraTripodBoss(const char* pName) : CameraTower(pName), mAngleY() {
}

CamTranslatorBase* CameraTripodBoss::createTranslator() {
    return new CamTranslatorTripodBoss(this);
}

void CameraTripodBoss::arrangeRound() {
    TVec3f offset = CameraLocalUtil::getPos(this) - CameraLocalUtil::getWatchPos(this);

    TQuat4f rot;
    rot.setRotate(TVec3f(0.0f, 1.0f, 0.0f), mAngleY);
    rot.transform(offset);

    CameraLocalUtil::setPos(this, CameraLocalUtil::getWatchPos(this) + offset);
    CameraTower::arrangeRound();
}
