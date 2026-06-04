#include "Game/Boss/KoopaBattleVs3Lv1.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/Boss/KoopaStateAttackFireShort.hpp"
#include "Game/Boss/KoopaStateAttackShockWave.hpp"
#include "Game/Boss/KoopaStateChaseRoll.hpp"
#include "Game/Boss/KoopaStateDamageEscape.hpp"
#include "Game/Boss/KoopaStateGuard.hpp"
#include "Game/Boss/KoopaStateJumpAway.hpp"

namespace NrvKoopaBattleVs3Lv1 {
    NEW_NERVE(KoopaBattleVs3Lv1NrvChaseRoll, KoopaBattleVs3Lv1, ChaseRoll);
    NEW_NERVE(KoopaBattleVs3Lv1NrvAttackFire, KoopaBattleVs3Lv1, AttackFire);
    NEW_NERVE(KoopaBattleVs3Lv1NrvAttackShockWave, KoopaBattleVs3Lv1, AttackShockWave);
    NEW_NERVE(KoopaBattleVs3Lv1NrvDamageReverse, KoopaBattleVs3Lv1, DamageReverse);
    NEW_NERVE(KoopaBattleVs3Lv1NrvJumpAway, KoopaBattleVs3Lv1, JumpAway);
    NEW_NERVE(KoopaBattleVs3Lv1NrvGuard, KoopaBattleVs3Lv1, Guard);
    NEW_NERVE(KoopaBattleVs3Lv1NrvRecover, KoopaBattleVs3Lv1, Recover);
}  // namespace NrvKoopaBattleVs3Lv1

KoopaBattleVs3Lv1::KoopaBattleVs3Lv1(Koopa* pKoopa) : KoopaBattleBase("クッパ戦闘（Ｖｓ３Ｌｖ１）", pKoopa), _1C(), _20() {
}

KoopaBattleVs3Lv1::~KoopaBattleVs3Lv1() {
}

void KoopaBattleVs3Lv1::init() {
    KoopaBattleBase::init();
    initNerve(&NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvAttackShockWave::sInstance);
    MR::initActorStateKeeper(this, 16);

    _1C = new KoopaStateChaseRoll(mHost);
    _20 = new KoopaStateAttackShockWave(mHost);

    MR::initActorState(this, _1C, &NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvChaseRoll::sInstance, "ChaseRoll");
    MR::initActorState(this, new KoopaStateAttackFireShort(mHost), &NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvAttackFire::sInstance, "AttackFire");
    MR::initActorState(this, _20, &NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvAttackShockWave::sInstance, "AttackShockWave");
    MR::initActorState(this, mStateDamageEscape, &NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvDamageReverse::sInstance, "Damage");
    MR::initActorState(this, new KoopaStateJumpAway(mHost), &NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvJumpAway::sInstance, "JumpAway");
    MR::initActorState(this, mStateGuard, &NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvGuard::sInstance, "Guard");
}

void KoopaBattleVs3Lv1::appear() {
    mIsDead = false;

    setNerve(&NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvAttackShockWave::sInstance);
}

void KoopaBattleVs3Lv1::exeChaseRoll() {
    if (!MR::updateActorState(this)) {
        return;
    }

    if (MR::isPlayerDamaging()) {
        setNerve(&NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvJumpAway::sInstance);
    } else {
        setNerve(&NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvAttackShockWave::sInstance);
    }
}

void KoopaBattleVs3Lv1::exeAttackShockWave() {
    if (!MR::updateActorState(this)) {
        return;
    }

    if (MR::isPlayerDamaging()) {
        setNerve(&NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvJumpAway::sInstance);
    } else {
        setNerve(&NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvChaseRoll::sInstance);
    }
}

void KoopaBattleVs3Lv1::exeDamageReverse() {
    if (MR::isFirstStep(this)) {
        mStateDamageEscape->appear();
        mStateDamageEscape->startDamageReverse();
    }

    if (mStateDamageEscape->update()) {
        setNerve(&NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvAttackShockWave::sInstance);
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
        setNerve(&NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvRecover::sInstance);
    }
}

bool KoopaBattleVs3Lv1::tryCalcAndSetBaseMtx() {
    if (isNerve(&NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvChaseRoll::sInstance) && _1C->tryCalcAndSetBaseMtx()) {
        return true;
    }

    return false;
}

bool KoopaBattleVs3Lv1::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvChaseRoll::sInstance)) {
        if (!_1C->attackSensor(pSender, pReceiver)) {
            return KoopaFunction::tryKoopaPushPlayer(pSender, pReceiver);
        }
    } else {
        if (isNerve(&NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvAttackShockWave::sInstance)) {
            return _20->attackSensor(pSender, pReceiver);
        }

        if (!isNerve(&NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvJumpAway::sInstance) || !KoopaFunction::tryKoopaAttackMapObj(pSender, pReceiver)) {
            if (isNerve(&NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvDamageReverse::sInstance)) {
                return mStateDamageEscape->attackSensor(pSender, pReceiver);
            } else {
                if (!KoopaFunction::tryKoopaPushPlayer(pSender, pReceiver)) {
                    return KoopaFunction::tryKoopaBodyAttackPlayer(pSender, pReceiver);
                }
            }
        }
    }

    return false;
}

bool KoopaBattleVs3Lv1::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvChaseRoll::sInstance) && _1C->tryDamage(msg, pSender, pReceiver)) {
        setNerve(&NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvDamageReverse::sInstance);

        return true;
    }

    if (isNerve(&NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvDamageReverse::sInstance)) {
        return mStateDamageEscape->tryDamage(msg, pSender, pReceiver);
    }

    if ((isNerve(&NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvAttackFire::sInstance) ||
         isNerve(&NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvAttackShockWave::sInstance) ||
         (isNerve(&NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvChaseRoll::sInstance) && _1C->isEnableGuard()) ||
         isNerve(&NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvJumpAway::sInstance) ||
         isNerve(&NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvRecover::sInstance)) &&
        mStateGuard->tryStart(msg, pSender, pReceiver)) {
        setNerve(&NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvGuard::sInstance);

        return true;
    }

    if (KoopaFunction::tryKoopaReflectStarPiece(msg, pSender, pReceiver)) {
        return true;
    }

    return false;
}

void KoopaBattleVs3Lv1::exeAttackFire() {
    MR::updateActorStateAndNextNerve(this, &NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvAttackShockWave::sInstance);
}

void KoopaBattleVs3Lv1::exeJumpAway() {
    MR::updateActorStateAndNextNerve(this, &NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvAttackShockWave::sInstance);
}

void KoopaBattleVs3Lv1::exeGuard() {
    if (MR::updateActorState(this)) {
        setNerve(&NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvAttackShockWave::sInstance);
    }
}

void KoopaBattleVs3Lv1::exeRecover() {
    updateRecover(&NrvKoopaBattleVs3Lv1::KoopaBattleVs3Lv1NrvAttackShockWave::sInstance);
}
