#include "Game/Boss/PoltaBattleLv1.hpp"
#include "Game/Boss/Polta.hpp"
#include "Game/Boss/PoltaActionBase.hpp"
#include "Game/Boss/PoltaFunction.hpp"
#include "Game/Boss/PoltaStateGenerateRock.hpp"
#include "Game/Boss/PoltaStateGroundRockAttack.hpp"
#include "Game/Boss/PoltaStateStagger.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace NrvPoltaBattleLv1 {
    NEW_NERVE(PoltaBattleLv1NrvWait, PoltaBattleLv1, Wait);
    NEW_NERVE(PoltaBattleLv1NrvGenerateGroundRock, PoltaBattleLv1, GenerateGroundRock);
    NEW_NERVE(PoltaBattleLv1NrvGenerateRock, PoltaBattleLv1, GenerateRock);
    NEW_NERVE(PoltaBattleLv1NrvDamageBody, PoltaBattleLv1, DamageBody);
    NEW_NERVE(PoltaBattleLv1NrvBreakBody, PoltaBattleLv1, BreakBody);
    NEW_NERVE_ONEND(PoltaBattleLv1NrvStagger, PoltaBattleLv1, Stagger, Stagger);
    NEW_NERVE(PoltaBattleLv1NrvDamageCore, PoltaBattleLv1, DamageCore);
};  // namespace NrvPoltaBattleLv1

PoltaBattleLv1::PoltaBattleLv1(Polta* pPolta) : PoltaActionBase("ポルタ1戦目", pPolta), mPoltaHealth(3), _24(0) {
    initNerve(&NrvPoltaBattleLv1::PoltaBattleLv1NrvWait::sInstance);
    mStateStagger = new PoltaStateStagger(pPolta);
    mStateGroundRockAttack = new PoltaStateGroundRockAttack(pPolta);
    mStateGroundRockAttack->_10 = 4;
    mStateGenerateRock = new PoltaStateGenerateRock(pPolta);
    mStateGenerateRock->mMaxIndexIntoPattern = 5;
    mStateGenerateRock->mEndDelayStep = 120;
}

void PoltaBattleLv1::appear() {
    if (MR::isDead(mPoltaPtr)) {
        mPoltaPtr->makeActorAppeared();
    }

    mIsDead = false;
    mPoltaHealth = 3;
    PoltaFunction::setBodyHP(mPoltaPtr, 3);
    PoltaFunction::killLeftArm(mPoltaPtr);
    PoltaFunction::killRightArm(mPoltaPtr);
    setNerve(&NrvPoltaBattleLv1::PoltaBattleLv1NrvWait::sInstance);
    _24 = 0;
    PoltaFunction::emitEffectShadow(mPoltaPtr);
}

void PoltaBattleLv1::control() {
    mPoltaPtr->updatePose(0.2f, 0.4f);
    if (isNerve(&NrvPoltaBattleLv1::PoltaBattleLv1NrvWait::sInstance) ||
        isNerve(&NrvPoltaBattleLv1::PoltaBattleLv1NrvGenerateGroundRock::sInstance) ||
        isNerve(&NrvPoltaBattleLv1::PoltaBattleLv1NrvGenerateRock::sInstance) ||
        isNerve(&NrvPoltaBattleLv1::PoltaBattleLv1NrvDamageBody::sInstance) || isNerve(&NrvPoltaBattleLv1::PoltaBattleLv1NrvBreakBody::sInstance)) {
        MR::startLevelSound(mPoltaPtr, "SE_BM_LV_POLTA_IN_BATTLE_ROCK", -1, -1, -1);
    } else if (isNerve(&NrvPoltaBattleLv1::PoltaBattleLv1NrvStagger::sInstance) ||
               isNerve(&NrvPoltaBattleLv1::PoltaBattleLv1NrvDamageCore::sInstance)) {
        MR::startLevelSound(mPoltaPtr, "SE_BM_LV_POLTA_CORE_ESCAPE", -1, -1, -1);
    }
}

void PoltaBattleLv1::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if ((!isEnableScream() || !PoltaFunction::attackScreamSensor(mPoltaPtr, pSender, pReceiver)) && isEnableSensor(pSender)) {
        if (MR::isSensorPlayer(pReceiver)) {
            if (!MR::sendMsgEnemyAttackStrong(pReceiver, pSender)) {
                MR::sendMsgPush(pReceiver, pSender);
            }
            return;
        }
        MR::sendMsgPush(pReceiver, pSender);
    }
}

bool PoltaBattleLv1::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return !isEnableSensor(pReceiver) ? false : MR::isMsgStarPieceReflect(msg);
}

bool PoltaBattleLv1::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!isEnableDamage()) {
        return false;
    }

    if (!isEnableSensor(pReceiver)) {
        return false;
    }

    if (MR::isMsgExplosionAttack(msg)) {
        if (PoltaFunction::isBodySensor(mPoltaPtr, pReceiver)) {
            addDamageBody();
            if (mPoltaHealth <= 0) {
                setNerve(&NrvPoltaBattleLv1::PoltaBattleLv1NrvBreakBody::sInstance);
            } else {
                setNerve(&NrvPoltaBattleLv1::PoltaBattleLv1NrvDamageBody::sInstance);
            }
            return true;
        } else if (PoltaFunction::isCoreSensor(mPoltaPtr, pReceiver)) {
            setNerve(&NrvPoltaBattleLv1::PoltaBattleLv1NrvDamageCore::sInstance);
            return true;
        }
    }
    return false;
}

bool PoltaBattleLv1::isEnableSensor(const HitSensor* pSensor) const {
    if (isNerve(&NrvPoltaBattleLv1::PoltaBattleLv1NrvStagger::sInstance) && !mStateStagger->isEnableSensor()) {
        return false;
    }

    if (PoltaFunction::isBodySensor(mPoltaPtr, pSensor)) {
        return mPoltaHealth > 0;
    }

    if (PoltaFunction::isCoreSensor(mPoltaPtr, pSensor)) {
        return mPoltaHealth <= 0;
    }

    if (PoltaFunction::isHeadSensor(mPoltaPtr, pSensor)) {
        return mPoltaHealth <= 0;
    }
    return false;
}

void PoltaBattleLv1::exeWait() {
    updateWait();
    if (MR::isGreaterStep(this, 90)) {
        if (_24 == 2 && PoltaFunction::getCountDeadGroundRock(mPoltaPtr) >= 4) {
            setNerve(&NrvPoltaBattleLv1::PoltaBattleLv1NrvGenerateGroundRock::sInstance);
        } else {
            setNerve(&NrvPoltaBattleLv1::PoltaBattleLv1NrvGenerateRock::sInstance);
        }
        _24 = (_24 + 4) % 3;
    }
}

void PoltaBattleLv1::exeBreakBody() {
    if (MR::isFirstStep(this)) {
        mPoltaPtr->appearStarPiece(10);
    }
    if (updateBreakBody()) {
        setNerve(&NrvPoltaBattleLv1::PoltaBattleLv1NrvStagger::sInstance);
    }
}

void PoltaBattleLv1::exeStagger() {
    if (MR::isFirstStep(this)) {
        mStateStagger->setActionName("StaggerLv1");
        MR::setStageBGMState(2, 60);
        MR::startSound(mPoltaPtr, "SE_BV_POLTA_RUN_AWAY", -1, -1);
    }
    MR::updateActorState(this, mStateStagger);
}

void PoltaBattleLv1::exeDamageCore() {
    if (MR::isFirstStep(this)) {
        MR::startSystemSE("SE_SY_VS_BOSS_DAMAGE_3", -1, -1);
        MR::startSound(mPoltaPtr, "SE_BV_POLTA_CORE_DAMAGE", -1, -1);
        MR::startSound(mPoltaPtr, "SE_BM_POLTA_CORE_DAMAGE", -1, -1);
    }
    if (updateDamageCore()) {
        kill();
    }
}

bool PoltaBattleLv1::isEnableDamage() const {
    if (isNerve(&NrvPoltaBattleLv1::PoltaBattleLv1NrvBreakBody::sInstance) || isNerve(&NrvPoltaBattleLv1::PoltaBattleLv1NrvDamageCore::sInstance)) {
        return false;
    }
    return true;
}

bool PoltaBattleLv1::isEnableScream() const {
    return isNerve(&NrvPoltaBattleLv1::PoltaBattleLv1NrvBreakBody::sInstance);
}

void PoltaBattleLv1::addDamageBody() {
    if (mPoltaHealth > 0) {
        mPoltaHealth--;
        PoltaFunction::setBodyHP(mPoltaPtr, mPoltaHealth);
    }
}

inline void PoltaBattleLv1::endStagger() {
    MR::setStageBGMState(1, 90);
}

inline void PoltaBattleLv1::exeDamageBody() {
    if (updateDamageBody(mPoltaHealth == 2)) {
        setNerve(&NrvPoltaBattleLv1::PoltaBattleLv1NrvWait::sInstance);
    }
}

inline void PoltaBattleLv1::exeGenerateRock() {
    MR::updateActorStateAndNextNerve(this, mStateGenerateRock, &NrvPoltaBattleLv1::PoltaBattleLv1NrvWait::sInstance);
}

inline void PoltaBattleLv1::exeGenerateGroundRock() {
    MR::updateActorStateAndNextNerve(this, mStateGroundRockAttack, &NrvPoltaBattleLv1::PoltaBattleLv1NrvWait::sInstance);
}
