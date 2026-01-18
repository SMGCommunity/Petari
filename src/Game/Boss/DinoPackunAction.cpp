#include "Game/Boss/DinoPackunAction.hpp"
#include "Game/Boss/DinoPackun.hpp"
#include "Game/LiveActor/EffectKeeper.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Util.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include <JSystem/JGeometry/TVec.hpp>

DinoPackunAction::DinoPackunAction(const char* pName, DinoPackun* pParent) : ActorStateBase< DinoPackun >(pName) {
    mParent = pParent;
    _10 = 1.0f;
}

bool DinoPackunAction::sendBlowAttackMessage(HitSensor* pSender, HitSensor* pReceiver, bool a3) {
    TVec3f direction;
    MR::calcSensorDirection(&direction, pSender, pReceiver);

    if (mParent->mVelocity.dot(direction) >= 0.0f) {
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
    MR::calcSideVec(&side, mParent);
    TVec3f stack_8;
    stack_8.setPS(side);
}
*/

bool DinoPackunAction::updateWalk(s32 a1, f32 a2, s32 a3) {
    MR::addVelocityMoveToDirection(mParent, mParent->_E8, a2);

    if (a2 > 1.2f) {
        mParent->updateRunVelocity();
    } else {
        mParent->updateNormalVelocity();
    }

    DinoPackun* parent = mParent;
    parent->updateFootPrintNerve(getNerveStep(), a3);
    if (MR::isGreaterStep(this, a1)) {
        selectTurnDirection();
        return true;
    }

    return false;
}

bool DinoPackunAction::updateChase(s32 a1, f32 a2, f32 a3, f32 a4, s32 a5, s32 a6) {
    if (MR::isFaceToPlayerDegree(mParent, mParent->_E8, a2)) {
        MR::turnDirectionToPlayerDegree(mParent, &mParent->_E8, a3);
    }

    DinoPackun* parent = mParent;
    parent->updateFootPrintNerve(getNerveStep(), a6);
    MR::addVelocityMoveToDirection(mParent, mParent->_E8, (a4 * MR::calcNerveRate(this, a5)));
    mParent->updateRunVelocity();
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
    mParent->updateNormalVelocity();

    if (MR::isBckOneTimeAndStopped(mParent)) {
        MR::startBck(mParent, "Wait", nullptr);
    }

    if (MR::isGreaterStep(this, 60)) {
        return true;
    }

    return false;
}

bool DinoPackunAction::updateFind(s32 a1, f32 a2) {
    if (MR::isFirstStep(this)) {
        MR::startBck(mParent, "Find", nullptr);
        MR::startSound(mParent, "SE_BV_PAKKUN_FIND", -1, -1);
        MR::startSound(mParent, "SE_BM_D_PAKKUN_SLAVER", -1, -1);
    }

    if (MR::isStep(this, a1)) {
        MR::startSound(mParent, "SE_BM_D_PAKKUN_LAND", -1, -1);
    }

    MR::turnDirectionToPlayerDegree(mParent, &mParent->_E8, a2);
    mParent->updateNormalVelocity();

    if (MR::isBckStopped(mParent)) {
        return true;
    }

    return false;
}

bool DinoPackunAction::updateCoolDown(s32 a1) {
    if (MR::isFirstStep(this)) {
        MR::startBck(mParent, "CoolDown", nullptr);
    }

    mParent->updateNormalVelocity();

    if (MR::isGreaterStep(this, a1)) {
        return true;
    }

    return false;
}

bool DinoPackunAction::updateAttackHit() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mParent, "AttackHit");
        MR::startSound(mParent, "SE_BV_D_PAKKUN_GLAD", -1, -1);
        MR::startSound(mParent, "SE_BM_D_PAKKUN_SLAVER", -1, -1);
    }

    mParent->updateNormalVelocity();

    if (MR::isActionEnd(mParent)) {
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
