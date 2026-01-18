#include "Game/Boss/DinoPackunAction.hpp"
#include "Game/Boss/DinoPackun.hpp"
#include "Game/LiveActor/EffectKeeper.hpp"
#include "Game/Util.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
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
