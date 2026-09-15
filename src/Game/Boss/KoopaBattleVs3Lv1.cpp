#include "Game/Boss/KoopaBattleVs3Lv1.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/Boss/KoopaStateAttackFireShort.hpp"
#include "Game/Boss/KoopaStateAttackShockWave.hpp"
#include "Game/Boss/KoopaStateChaseRoll.hpp"
#include "Game/Boss/KoopaStateDamageEscape.hpp"
#include "Game/Boss/KoopaStateGuard.hpp"
#include "Game/Boss/KoopaStateJumpAway.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"

namespace NrvKoopaBattleVs3Lv1 {
    NEW_NERVE(KoopaBattleVs3Lv1NrvChaseRoll, KoopaBattleVs3Lv1, ChaseRoll);
    NEW_NERVE(KoopaBattleVs3Lv1NrvAttackFire, KoopaBattleVs3Lv1, AttackFire);
    NEW_NERVE(KoopaBattleVs3Lv1NrvAttackShockWave, KoopaBattleVs3Lv1, AttackShockWave);
    NEW_NERVE(KoopaBattleVs3Lv1NrvDamageReverse, KoopaBattleVs3Lv1, DamageReverse);
    NEW_NERVE(KoopaBattleVs3Lv1NrvJumpAway, KoopaBattleVs3Lv1, JumpAway);
    NEW_NERVE(KoopaBattleVs3Lv1NrvGuard, KoopaBattleVs3Lv1, Guard);
    NEW_NERVE(KoopaBattleVs3Lv1NrvRecover, KoopaBattleVs3Lv1, Recover);
};  // namespace NrvKoopaBattleVs3Lv1

KoopaBattleVs3Lv1::KoopaBattleVs3Lv1(Koopa* pKoopa)
    : KoopaBattleBase("クッパ戦闘（Ｖｓ３Ｌｖ１）", pKoopa), mStateChaseRoll(), mStateAttackShockWave() {
}

void KoopaBattleVs3Lv1::init() {
    KoopaBattleBase::init();
    initNerve(GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvAttackShockWave));
    MR::initActorStateKeeper(this, 16);

    mStateChaseRoll = new KoopaStateChaseRoll(mHost);
    mStateAttackShockWave = new KoopaStateAttackShockWave(mHost);

    MR::initActorState(this, mStateChaseRoll, GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvChaseRoll), "ChaseRoll");
    MR::initActorState(this, new KoopaStateAttackFireShort(mHost), GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvAttackFire), "AttackFire");
    MR::initActorState(this, mStateAttackShockWave, GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvAttackShockWave), "AttackShockWave");
    MR::initActorState(this, mStateDamageEscape, GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvDamageReverse), "Damage");
    MR::initActorState(this, new KoopaStateJumpAway(mHost), GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvJumpAway), "JumpAway");
    MR::initActorState(this, mStateGuard, GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvGuard), "Guard");
}

void KoopaBattleVs3Lv1::appear() {
    mIsDead = false;

    setNerve(GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvAttackShockWave));
}

void KoopaBattleVs3Lv1::exeChaseRoll() {
    if (!MR::updateActorState(this)) {
        return;
    }

    if (MR::isPlayerDamaging()) {
        setNerve(GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvJumpAway));
    } else {
        setNerve(GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvAttackShockWave));
    }
}

void KoopaBattleVs3Lv1::exeAttackShockWave() {
    if (!MR::updateActorState(this)) {
        return;
    }

    if (MR::isPlayerDamaging()) {
        setNerve(GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvJumpAway));
    } else {
        setNerve(GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvChaseRoll));
    }
}

void KoopaBattleVs3Lv1::exeDamageReverse() {
    if (MR::isFirstStep(this)) {
        mStateDamageEscape->appear();
        mStateDamageEscape->startDamageReverse();
    }

    if (mStateDamageEscape->update()) {
        setNerve(GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvAttackShockWave));
        return;
    }

    if (!mStateDamageEscape->isDownEnd()) {
        return;
    }

    if (KoopaFunction::isKoopaAngry(mHost)) {
        KoopaFunction::changeBgmStateNormal(120);
        kill();
    } else {
        KoopaFunction::startKoopaAngry(mHost);
        setNerve(GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvRecover));
    }
}

bool KoopaBattleVs3Lv1::tryCalcAndSetBaseMtx() {
    if (isNerve(GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvChaseRoll)) && mStateChaseRoll->tryCalcAndSetBaseMtx()) {
        return true;
    }

    return false;
}

void KoopaBattleVs3Lv1::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvChaseRoll))) {
        if (!mStateChaseRoll->attackSensor(pSender, pReceiver)) {
            KoopaFunction::tryKoopaPushPlayer(pSender, pReceiver);
        }
    } else if (isNerve(GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvAttackShockWave))) {
        mStateAttackShockWave->attackSensor(pSender, pReceiver);
    } else if (!isNerve(GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvJumpAway)) || !KoopaFunction::tryKoopaAttackMapObj(pSender, pReceiver)) {
        if (isNerve(GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvDamageReverse))) {
            mStateDamageEscape->attackSensor(pSender, pReceiver);
        } else if (!KoopaFunction::tryKoopaPushPlayer(pSender, pReceiver) && KoopaFunction::tryKoopaBodyAttackPlayer(pSender, pReceiver)) {
            return;
        }
    }
}

bool KoopaBattleVs3Lv1::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvChaseRoll)) && mStateChaseRoll->tryDamage(msg, pSender, pReceiver)) {
        setNerve(GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvDamageReverse));

        return true;
    }

    if (isNerve(GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvDamageReverse))) {
        return mStateDamageEscape->tryDamage(msg, pSender, pReceiver);
    }

    if ((isNerve(GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvAttackFire)) ||
         isNerve(GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvAttackShockWave)) ||
         (isNerve(GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvChaseRoll)) && mStateChaseRoll->isEnableGuard()) ||
         isNerve(GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvJumpAway)) || isNerve(GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvRecover))) &&
        mStateGuard->tryStart(msg, pSender, pReceiver)) {
        setNerve(GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvGuard));

        return true;
    }

    if (KoopaFunction::tryKoopaReflectStarPiece(msg, pSender, pReceiver)) {
        return true;
    }

    return false;
}

void KoopaBattleVs3Lv1::exeAttackFire() {
    MR::updateActorStateAndNextNerve(this, GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvAttackShockWave));
}

void KoopaBattleVs3Lv1::exeJumpAway() {
    MR::updateActorStateAndNextNerve(this, GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvAttackShockWave));
}

void KoopaBattleVs3Lv1::exeGuard() {
    if (MR::updateActorState(this)) {
        setNerve(GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvAttackShockWave));
    }
}

void KoopaBattleVs3Lv1::exeRecover() {
    updateRecover(GET_NERVE(KoopaBattleVs3Lv1, KoopaBattleVs3Lv1NrvAttackShockWave));
}

KoopaBattleVs3Lv1::~KoopaBattleVs3Lv1() {
}
