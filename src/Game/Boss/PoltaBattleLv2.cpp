#include "Game/Boss/PoltaBattleLv2.hpp"
#include "Game/Boss/Polta.hpp"
#include "Game/Boss/PoltaActionBase.hpp"
#include "Game/Boss/PoltaArm.hpp"
#include "Game/Boss/PoltaFunction.hpp"
#include "Game/Boss/PoltaStateAttackGround.hpp"
#include "Game/Boss/PoltaStateGenerateRock.hpp"
#include "Game/Boss/PoltaStateGroundRockAttack.hpp"
#include "Game/Boss/PoltaStatePunch.hpp"
#include "Game/Boss/PoltaStateStagger.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"

namespace NrvPoltaBattleLv2 {
    NEW_NERVE(PoltaBattleLv2NrvWait, PoltaBattleLv2, Wait);
    NEW_NERVE(PoltaBattleLv2NrvPunch, PoltaBattleLv2, Punch);
    NEW_NERVE(PoltaBattleLv2NrvAttackGround, PoltaBattleLv2, AttackGround);
    NEW_NERVE(PoltaBattleLv2NrvGenerateGroundRock, PoltaBattleLv2, GenerateGroundRock);
    NEW_NERVE(PoltaBattleLv2NrvGenerateRock, PoltaBattleLv2, GenerateRock);
    NEW_NERVE(PoltaBattleLv2NrvDamageBody, PoltaBattleLv2, DamageBody);
    NEW_NERVE(PoltaBattleLv2NrvBreakBody, PoltaBattleLv2, BreakBody);
    NEW_NERVE_ONEND(PoltaBattleLv2NrvStagger, PoltaBattleLv2, Stagger, Stagger);
    NEW_NERVE(PoltaBattleLv2NrvDamageCore, PoltaBattleLv2, DamageCore);
};  // namespace NrvPoltaBattleLv2

PoltaBattleLv2::PoltaBattleLv2(Polta* pPolta) : PoltaActionBase("ボステレサ2戦目", pPolta), mPoltaHealth(3), _2C(0) {
    initNerve(&NrvPoltaBattleLv2::PoltaBattleLv2NrvWait::sInstance);
    mStateAttackGround = new PoltaStateAttackGround(pPolta);
    mStatePunch = new PoltaStatePunch(pPolta);
    mStateStagger = new PoltaStateStagger(pPolta);
    mStateGroundRockAttack = new PoltaStateGroundRockAttack(pPolta);
    mStateGroundRockAttack->_10 = 8;
    mStateGenerateRock = new PoltaStateGenerateRock(pPolta);
    mStateGenerateRock->mMaxIndexIntoPattern = 7;
    mStateGenerateRock->mEndDelayStep = 180;
}

void PoltaBattleLv2::appear() {
    if (MR::isDead(getHost())) {
        getHost()->makeActorAppeared();
    }
    mIsDead = 0;
    mPoltaHealth = 3;
    PoltaFunction::setBodyHP(getHost(), 3);
    PoltaFunction::startArm(getHost());
    setNerve(&NrvPoltaBattleLv2::PoltaBattleLv2NrvWait::sInstance);
    PoltaFunction::emitEffectShadow(getHost());
}

void PoltaBattleLv2::control() {
    getHost()->updatePose(0.2f, 0.4f);
    if (isNerve(&NrvPoltaBattleLv2::PoltaBattleLv2NrvWait::sInstance) ||
        isNerve(&NrvPoltaBattleLv2::PoltaBattleLv2NrvGenerateGroundRock::sInstance) ||
        isNerve(&NrvPoltaBattleLv2::PoltaBattleLv2NrvGenerateRock::sInstance) ||
        isNerve(&NrvPoltaBattleLv2::PoltaBattleLv2NrvDamageBody::sInstance) || isNerve(&NrvPoltaBattleLv2::PoltaBattleLv2NrvBreakBody::sInstance)) {
        MR::startLevelSound(getHost(), "SE_BM_LV_POLTA_IN_BATTLE_ROCK", -1, -1, -1);
    } else if (isNerve(&NrvPoltaBattleLv2::PoltaBattleLv2NrvStagger::sInstance) ||
               isNerve(&NrvPoltaBattleLv2::PoltaBattleLv2NrvDamageCore::sInstance)) {
        MR::startLevelSound(getHost(), "SE_BM_LV_POLTA_CORE_ESCAPE", -1, -1, -1);
    }
}

void PoltaBattleLv2::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isEnableScream() && PoltaFunction::attackScreamSensor(getHost(), pSender, pReceiver)) {
        return;
    }
    if (isEnableSensor(pSender)) {
        if (MR::isSensorPlayer(pReceiver)) {
            if (isEnableAttack(pSender)) {
                if (isNerve(&NrvPoltaBattleLv2::PoltaBattleLv2NrvAttackGround::sInstance) && PoltaFunction::isArmSensor(getHost(), pSender)) {
                    if (MR::sendArbitraryMsg(ACTMES_ENEMY_ATTACK_CRUSH, pReceiver, pSender)) {
                        return;
                    }
                } else if (MR::sendMsgEnemyAttackStrong(pReceiver, pSender)) {
                    return;
                }
            }
            MR::sendMsgPush(pReceiver, pSender);
            return;
        }
        if (isEnableAttack(pSender) && PoltaFunction::isArmSensor(getHost(), pSender)) {
            MR::sendArbitraryMsg(ACTMES_DISPERSE_BOMB_TERESA, pReceiver, pSender);
            MR::sendArbitraryMsg(ACTMES_BREAK_POLTA_GROUND_ROCK, pReceiver, pSender);
        } else {
            MR::sendArbitraryMsg(ACTMES_BREAK_POLTA_GROUND_ROCK, pReceiver, pSender);
            MR::sendMsgPush(pReceiver, pSender);
        }
    }
}

bool PoltaBattleLv2::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return !isEnableSensor(pReceiver) ? false : MR::isMsgStarPieceReflect(msg);
}

bool PoltaBattleLv2::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!isEnableDamage()) {
        return false;
    }
    if (!isEnableSensor(pReceiver)) {
        return false;
    }
    if (MR::isMsgExplosionAttack(msg)) {
        if (PoltaFunction::isBodySensor(getHost(), pReceiver)) {
            addDamageBody();
            if (mPoltaHealth <= 0) {
                setNerve(&NrvPoltaBattleLv2::PoltaBattleLv2NrvBreakBody::sInstance);
            } else {
                setNerve(&NrvPoltaBattleLv2::PoltaBattleLv2NrvDamageBody::sInstance);
            }
            return true;
        }
        if (PoltaFunction::isLeftArmSensor(getHost(), pReceiver)) {
            PoltaFunction::damageLeftArm(getHost());
            return true;
        }
        if (PoltaFunction::isRightArmSensor(getHost(), pReceiver)) {
            PoltaFunction::damageRightArm(getHost());
            return true;
        }
        if (PoltaFunction::isCoreSensor(getHost(), pReceiver)) {
            setNerve(&NrvPoltaBattleLv2::PoltaBattleLv2NrvDamageCore::sInstance);
            return true;
        }
    }
    return false;
}

bool PoltaBattleLv2::tryAttackGround() {
    if (!PoltaFunction::isEnableAttackLeftArm(getHost()) && !PoltaFunction::isEnableAttackRightArm(getHost())) {
        return false;
    }
    if (!MR::isFaceToPlayerHorizontalDegree(getHost(), getHost()->_C4, 45.0f)) {
        return false;
    }
    if (MR::isNearPlayer(getHost(), 1500.0f)) {
        setNerve(&NrvPoltaBattleLv2::PoltaBattleLv2NrvAttackGround::sInstance);
        return true;
    } else if (MR::isNearPlayer(getHost(), 3000.0f)) {
        if (!PoltaFunction::isEnableAttackRightArm(getHost())) {
            mStatePunch->mIsLeftArmActor = true;
            setNerve(&NrvPoltaBattleLv2::PoltaBattleLv2NrvPunch::sInstance);
        } else if (!PoltaFunction::isEnableAttackLeftArm(getHost())) {
            mStatePunch->mIsLeftArmActor = false;
            setNerve(&NrvPoltaBattleLv2::PoltaBattleLv2NrvPunch::sInstance);
        } else {
            if (MR::isPlayerLeftSide(getHost())) {
                mStatePunch->mIsLeftArmActor = true;
            } else {
                mStatePunch->mIsLeftArmActor = false;
            }
            setNerve(&NrvPoltaBattleLv2::PoltaBattleLv2NrvPunch::sInstance);
        }
        return true;
    }
    return false;
}

void PoltaBattleLv2::setNerveGenerateRock() {
    if (_2C == 2 && PoltaFunction::getCountDeadGroundRock(getHost()) >= 8) {
        setNerve(&NrvPoltaBattleLv2::PoltaBattleLv2NrvGenerateGroundRock::sInstance);
    } else {
        setNerve(&NrvPoltaBattleLv2::PoltaBattleLv2NrvGenerateRock::sInstance);
    }
    _2C = (_2C + 4) % 3;
}

bool PoltaBattleLv2::tryGenerateRock() {
    if (PoltaFunction::isBrokenLeftArm(getHost()) && PoltaFunction::isBrokenRightArm(getHost()) && MR::isGreaterStep(this, 120)) {
        setNerveGenerateRock();
        return true;
    }
    return false;
}

void PoltaBattleLv2::exeBreakBody() {
    if (MR::isFirstStep(this)) {
        getHost()->appearStarPiece(10);
    }
    if (updateBreakBody()) {
        setNerve(&NrvPoltaBattleLv2::PoltaBattleLv2NrvStagger::sInstance);
    }
}

void PoltaBattleLv2::exeStagger() {
    if (MR::isFirstStep(this)) {
        mStateStagger->setActionName("StaggerLv2");
        MR::setStageBGMState(2, 60);
        MR::startSound(getHost(), "SE_BV_POLTA_RUN_AWAY", -1, -1);
    }
    MR::updateActorState(this, mStateStagger);
}

void PoltaBattleLv2::exeDamageCore() {
    if (MR::isFirstStep(this)) {
        MR::startSystemSE("SE_SY_VS_BOSS_LAST_HIT", -1, -1);
        MR::startSound(getHost(), "SE_BV_POLTA_DAMAGE_LAST", -1, -1);
        MR::startSound(getHost(), "SE_BM_POLTA_LAST_DAMAGE", -1, -1);
        MR::stopStageBGM(60);
    }
    if (updateDamageCore()) {
        kill();
    }
}

bool PoltaBattleLv2::isEnableSensor(const HitSensor* pSensor) const {
    if (isNerve(&NrvPoltaBattleLv2::PoltaBattleLv2NrvStagger::sInstance) && !mStateStagger->isEnableSensor()) {
        return false;
    }

    if (PoltaFunction::isBodySensor(getHost(), pSensor)) {
        return mPoltaHealth > 0;
    }

    if (PoltaFunction::isCoreSensor(getHost(), pSensor)) {
        return mPoltaHealth <= 0;
    }

    if (PoltaFunction::isHeadSensor(getHost(), pSensor)) {
        return mPoltaHealth <= 0;
    }
    if (PoltaFunction::isLeftArmSensor(getHost(), pSensor)) {
        return getHost()->mLeftArm->isEnableHitSensor();
    }
    if (PoltaFunction::isRightArmSensor(getHost(), pSensor)) {
        return getHost()->mRightArm->isEnableHitSensor();
    }

    return false;
}

bool PoltaBattleLv2::isEnableAttack(const HitSensor* pSensor) const {
    if (PoltaFunction::isBodySensor(getHost(), pSensor)) {
        return true;
    }
    if (isNerve(&NrvPoltaBattleLv2::PoltaBattleLv2NrvAttackGround::sInstance) && mStateAttackGround->isEnableAttack(pSensor)) {
        return true;
    }
    if (isNerve(&NrvPoltaBattleLv2::PoltaBattleLv2NrvPunch::sInstance) && mStatePunch->isEnableAttack(pSensor)) {
        return true;
    }
    if (isNerve(&NrvPoltaBattleLv2::PoltaBattleLv2NrvStagger::sInstance) &&
        (PoltaFunction::isCoreSensor(getHost(), pSensor) || PoltaFunction::isHeadSensor(getHost(), pSensor))) {
        return true;
    }
    return false;
}

bool PoltaBattleLv2::isEnableDamage() const {
    if (isNerve(&NrvPoltaBattleLv2::PoltaBattleLv2NrvBreakBody::sInstance) || isNerve(&NrvPoltaBattleLv2::PoltaBattleLv2NrvDamageCore::sInstance)) {
        return false;
    }
    return true;
}

bool PoltaBattleLv2::isEnableScream() const {
    return isNerve(&NrvPoltaBattleLv2::PoltaBattleLv2NrvBreakBody::sInstance);
}

void PoltaBattleLv2::addDamageBody() {
    if (mPoltaHealth > 0) {
        mPoltaHealth--;
        PoltaFunction::setBodyHP(getHost(), mPoltaHealth);
    }
}

inline void PoltaBattleLv2::endStagger() {
    MR::setStageBGMState(1, 90);
}

inline void PoltaBattleLv2::exeDamageBody() {
    if (updateDamageBody(mPoltaHealth == 2)) {
        setNerve(&NrvPoltaBattleLv2::PoltaBattleLv2NrvWait::sInstance);
    }
}

inline void PoltaBattleLv2::exeGenerateRock() {
    MR::updateActorStateAndNextNerve(this, mStateGenerateRock, &NrvPoltaBattleLv2::PoltaBattleLv2NrvWait::sInstance);
}

inline void PoltaBattleLv2::exeGenerateGroundRock() {
    MR::updateActorStateAndNextNerve(this, mStateGroundRockAttack, &NrvPoltaBattleLv2::PoltaBattleLv2NrvWait::sInstance);
}

inline void PoltaBattleLv2::exeAttackGround() {
    if (MR::updateActorState(this, mStateAttackGround)) {
        setNerveGenerateRock();
    }
}

inline void PoltaBattleLv2::exePunch() {
    if (MR::updateActorState(this, mStatePunch)) {
        setNerveGenerateRock();
    }
}

inline void PoltaBattleLv2::exeWait() {
    updateWait();
    if (!tryGenerateRock()) {
        tryAttackGround();
    }
}
