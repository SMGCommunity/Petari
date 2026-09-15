#include "Game/Boss/KoopaBattleVs3Lv2.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/Boss/KoopaStateAttackFireShort.hpp"
#include "Game/Boss/KoopaStateAttackRoll.hpp"
#include "Game/Boss/KoopaStateDamageEscape.hpp"
#include "Game/Boss/KoopaStateGuard.hpp"
#include "Game/Boss/KoopaStateJumpAway.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace NrvKoopaBattleVs3Lv2 {
    NEW_NERVE(KoopaBattleVs3Lv2NrvAttackRoll, KoopaBattleVs3Lv2, AttackRoll);
    NEW_NERVE(KoopaBattleVs3Lv2NrvAttackFire, KoopaBattleVs3Lv2, AttackFire);
    NEW_NERVE(KoopaBattleVs3Lv2NrvDamageReverse, KoopaBattleVs3Lv2, DamageReverse);
    NEW_NERVE(KoopaBattleVs3Lv2NrvJumpAway, KoopaBattleVs3Lv2, JumpAway);
    NEW_NERVE(KoopaBattleVs3Lv2NrvGuard, KoopaBattleVs3Lv2, Guard);
    NEW_NERVE(KoopaBattleVs3Lv2NrvRecover, KoopaBattleVs3Lv2, Recover);
};  // namespace NrvKoopaBattleVs3Lv2

KoopaBattleVs3Lv2::KoopaBattleVs3Lv2(Koopa* pKoopa) : KoopaBattleBase("クッパ戦闘（Ｖｓ３Ｌｖ２）", pKoopa), mStateAttackRoll() {
}

void KoopaBattleVs3Lv2::init() {
    KoopaBattleBase::init();
    initNerve(GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvAttackFire));
    MR::initActorStateKeeper(this, 16);

    mStateAttackRoll = new KoopaStateAttackRoll(mHost);

    MR::initActorState(this, mStateAttackRoll, GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvAttackRoll), "AttackRoll");
    MR::initActorState(this, new KoopaStateAttackFireShort(mHost), GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvAttackFire), "AttackFire");
    MR::initActorState(this, mStateDamageEscape, GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvDamageReverse), "Damage");
    MR::initActorState(this, new KoopaStateJumpAway(mHost), GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvJumpAway), "JumpAway");
    MR::initActorState(this, mStateGuard, GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvGuard), "Guard");
}

void KoopaBattleVs3Lv2::appear() {
    mIsDead = false;

    setNerve(GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvAttackFire));
}

void KoopaBattleVs3Lv2::exeAttackRoll() {
    if (MR::updateActorState(this)) {
        if (MR::isPlayerDamaging()) {
            setNerve(GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvJumpAway));
        } else {
            setNerve(GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvAttackFire));
        }
    }
}

void KoopaBattleVs3Lv2::exeAttackFire() {
    if (MR::updateActorState(this)) {
        if (MR::isPlayerDamaging()) {
            setNerve(GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvJumpAway));
        } else {
            setNerve(GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvAttackRoll));
        }
    }
}

void KoopaBattleVs3Lv2::exeDamageReverse() {
    if (MR::isFirstStep(this)) {
        mStateDamageEscape->appear();
        mStateDamageEscape->startDamageReverse();
    }

    if (mStateDamageEscape->update()) {
        setNerve(GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvAttackFire));
        return;
    }

    if (!mStateDamageEscape->isDownEnd()) {
        return;
    }

    if (KoopaFunction::isKoopaAngry(mHost)) {
        MR::stopStageBGM(120);
        kill();
    } else {
        KoopaFunction::startKoopaAngry(mHost);
        setNerve(GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvRecover));
    }
}

bool KoopaBattleVs3Lv2::tryCalcAndSetBaseMtx() {
    if (isNerve(GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvAttackRoll)) && mStateAttackRoll->tryCalcAndSetBaseMtx()) {
        return true;
    }

    return false;
}

void KoopaBattleVs3Lv2::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvAttackRoll))) {
        if (!mStateAttackRoll->attackSensor(pSender, pReceiver)) {
            KoopaFunction::tryKoopaPushPlayer(pSender, pReceiver);
        }
    } else if (!isNerve(GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvJumpAway)) || !KoopaFunction::tryKoopaAttackMapObj(pSender, pReceiver)) {
        if (isNerve(GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvDamageReverse))) {
            mStateDamageEscape->attackSensor(pSender, pReceiver);
        } else if (!KoopaFunction::tryKoopaPushPlayer(pSender, pReceiver) && KoopaFunction::tryKoopaBodyAttackPlayer(pSender, pReceiver)) {
            return;
        }
    }
}

bool KoopaBattleVs3Lv2::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvDamageReverse))) {
        return mStateDamageEscape->tryDamage(msg, pSender, pReceiver);
    }

    if ((isNerve(GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvAttackFire)) ||
         (isNerve(GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvAttackRoll)) && mStateAttackRoll->isEnableGuard()) ||
         isNerve(GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvJumpAway)) || isNerve(GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvRecover))) &&
        mStateGuard->tryStart(msg, pSender, pReceiver)) {
        setNerve(GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvGuard));
        return true;
    }

    if (KoopaFunction::tryKoopaReflectStarPiece(msg, pSender, pReceiver)) {
        return true;
    }

    return false;
}

bool KoopaBattleVs3Lv2::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvDamageReverse))) {
        return mStateDamageEscape->tryDamage(msg, pSender, pReceiver);
    }

    if (isNerve(GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvAttackRoll)) && mStateAttackRoll->isDamage(msg, pSender, pReceiver)) {
        setNerve(GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvDamageReverse));
        return true;
    }

    if (isNerve(GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvAttackFire)) && MR::isMsgToEnemyAttackBlow(msg)) {
        setNerve(GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvDamageReverse));
        return true;
    }

    return false;
}

void KoopaBattleVs3Lv2::exeGuard() {
    if (MR::updateActorState(this)) {
        setNerve(GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvAttackFire));
    }
}

void KoopaBattleVs3Lv2::exeJumpAway() {
    MR::updateActorStateAndNextNerve(this, GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvAttackFire));
}

void KoopaBattleVs3Lv2::exeRecover() {
    updateRecover(GET_NERVE(KoopaBattleVs3Lv2, KoopaBattleVs3Lv2NrvAttackFire));
}

KoopaBattleVs3Lv2::~KoopaBattleVs3Lv2() {
}
