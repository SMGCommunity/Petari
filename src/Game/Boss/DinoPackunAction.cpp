#include "Game/Boss/DinoPackunAction.hpp"
#include "Game/Boss/DinoPackun.hpp"
#include "Game/LiveActor/EffectKeeper.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Util.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include <JSystem/JGeometry/TVec.hpp>

DinoPackunAction::DinoPackunAction(const char* pName, DinoPackun* pParent) : ActorStateBase< DinoPackun >(pName, pParent) {
    _10 = 1.0f;
}

bool DinoPackunAction::sendBlowAttackMessage(HitSensor* pSender, HitSensor* pReceiver, bool a3) {
    TVec3f direction;
    MR::calcSensorDirection(&direction, pSender, pReceiver);

    if (mHost->mVelocity.dot(direction) >= 0.0f) {
        if (a3) {
            if (MR::sendMsgEnemyAttackFireStrong(pReceiver, pSender)) {
                return true;
            }
        } else if (MR::sendMsgEnemyAttackMaximum(pReceiver, pSender)) {
            return true;
        }
    } else if (a3) {
        if (MR::sendMsgEnemyAttackFire(pReceiver, pSender)) {
            return true;
        }
    } else if (MR::sendMsgEnemyAttackStrong(pReceiver, pSender)) {
        return true;
    }

    return false;
}

bool DinoPackunAction::sendHitAttackMessage(HitSensor* pSender, HitSensor* pReceiver, bool a3) {
    if (a3) {
        if (MR::sendMsgEnemyAttackFire(pReceiver, pSender)) {
            return true;
        }
    } else if (MR::sendMsgEnemyAttackStrong(pReceiver, pSender)) {
        return true;
    }

    return false;
}

/*
void DinoPackunAction::updateTurn(s32 a1, f32 a2) {
    TVec3f side;
    MR::calcSideVec(&side, mActor);
    TVec3f stack_8;
    stack_8.setPS(side);
}
*/

bool DinoPackunAction::updateWalk(s32 a1, f32 a2, s32 a3) {
    MR::addVelocityMoveToDirection(mHost, mHost->_E8, a2);

    if (a2 > 1.2f) {
        mHost->updateRunVelocity();
    } else {
        mHost->updateNormalVelocity();
    }

    DinoPackun* parent = mHost;
    parent->updateFootPrintNerve(getNerveStep(), a3);
    if (MR::isGreaterStep(this, a1)) {
        selectTurnDirection();
        return true;
    }

    return false;
}

bool DinoPackunAction::updateChase(s32 a1, f32 a2, f32 a3, f32 a4, s32 a5, s32 a6) {
    if (MR::isFaceToPlayerDegree(mHost, mHost->_E8, a2)) {
        MR::turnDirectionToPlayerDegree(mHost, &mHost->_E8, a3);
    }

    DinoPackun* parent = mHost;
    parent->updateFootPrintNerve(getNerveStep(), a6);
    MR::addVelocityMoveToDirection(mHost, mHost->_E8, (a4 * MR::calcNerveRate(this, a5)));
    mHost->updateRunVelocity();
    if (MR::isGreaterStep(this, a1)) {
        return true;
    }

    return false;
}

void DinoPackunAction::selectTurnDirection() {
    if (MR::isHalfProbability()) {
        _10 = 1.0f;
    } else {
        _10 = -1.0f;
    }
}

DinoPackunAction::~DinoPackunAction() {}

bool DinoPackunAction::updateStart() {
    mHost->updateNormalVelocity();

    if (MR::isBckOneTimeAndStopped(mHost)) {
        MR::startBck(mHost, "Wait", nullptr);
    }

    if (MR::isGreaterStep(this, 60)) {
        return true;
    }

    return false;
}

bool DinoPackunAction::updateFind(s32 a1, f32 a2) {
    if (MR::isFirstStep(this)) {
        MR::startBck(mHost, "Find", nullptr);
        MR::startSound(mHost, "SE_BV_PAKKUN_FIND", -1, -1);
        MR::startSound(mHost, "SE_BM_D_PAKKUN_SLAVER", -1, -1);
    }

    if (MR::isStep(this, a1)) {
        MR::startSound(mHost, "SE_BM_D_PAKKUN_LAND", -1, -1);
    }

    MR::turnDirectionToPlayerDegree(mHost, &mHost->_E8, a2);
    mHost->updateNormalVelocity();

    if (MR::isBckStopped(mHost)) {
        return true;
    }

    return false;
}

bool DinoPackunAction::updateCoolDown(s32 a1) {
    if (MR::isFirstStep(this)) {
        MR::startBck(mHost, "CoolDown", nullptr);
    }

    mHost->updateNormalVelocity();

    if (MR::isGreaterStep(this, a1)) {
        return true;
    }

    return false;
}

bool DinoPackunAction::updateAttackHit() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "AttackHit");
        MR::startSound(mHost, "SE_BV_D_PAKKUN_GLAD", -1, -1);
        MR::startSound(mHost, "SE_BM_D_PAKKUN_SLAVER", -1, -1);
    }

    mHost->updateNormalVelocity();

    if (MR::isActionEnd(mHost)) {
        return true;
    }

    return false;
}

bool DinoPackunAction::receiveOtherMsgTail(u32, HitSensor*, HitSensor*) {
    return false;
}

bool DinoPackunAction::receiveMsgPlayerAttackTail(u32, HitSensor*, HitSensor*) {
    return false;
}

void DinoPackunAction::attackSensorTail(HitSensor*, HitSensor*) {
    return;
}

bool DinoPackunAction::receiveOtherMsg(u32, HitSensor*, HitSensor*) {
    return false;
}

bool DinoPackunAction::receiveMsgPush(HitSensor*, HitSensor*) {
    return false;
}

bool DinoPackunAction::receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*) {
    return false;
}

bool DinoPackunAction::receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*) {
    return false;
}

void DinoPackunAction::attackSensor(HitSensor*, HitSensor*) {
    return;
}
