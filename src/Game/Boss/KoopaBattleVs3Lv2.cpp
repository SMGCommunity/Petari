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
    initNerve(&NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvAttackFire::sInstance);
    MR::initActorStateKeeper(this, 16);

    mStateAttackRoll = new KoopaStateAttackRoll(mHost);

    MR::initActorState(this, mStateAttackRoll, &NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvAttackRoll::sInstance, "AttackRoll");
    MR::initActorState(this, new KoopaStateAttackFireShort(mHost), &NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvAttackFire::sInstance, "AttackFire");
    MR::initActorState(this, mStateDamageEscape, &NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvDamageReverse::sInstance, "Damage");
    MR::initActorState(this, new KoopaStateJumpAway(mHost), &NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvJumpAway::sInstance, "JumpAway");
    MR::initActorState(this, mStateGuard, &NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvGuard::sInstance, "Guard");
}

void KoopaBattleVs3Lv2::appear() {
    mIsDead = false;

    setNerve(&NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvAttackFire::sInstance);
}

void KoopaBattleVs3Lv2::exeAttackRoll() {
    if (MR::updateActorState(this)) {
        if (MR::isPlayerDamaging()) {
            setNerve(&NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvJumpAway::sInstance);
        } else {
            setNerve(&NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvAttackFire::sInstance);
        }
    }
}

void KoopaBattleVs3Lv2::exeAttackFire() {
    if (MR::updateActorState(this)) {
        if (MR::isPlayerDamaging()) {
            setNerve(&NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvJumpAway::sInstance);
        } else {
            setNerve(&NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvAttackRoll::sInstance);
        }
    }
}

void KoopaBattleVs3Lv2::exeDamageReverse() {
    if (MR::isFirstStep(this)) {
        mStateDamageEscape->appear();
        mStateDamageEscape->startDamageReverse();
    }

    if (mStateDamageEscape->update()) {
        setNerve(&NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvAttackFire::sInstance);
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
        setNerve(&NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvRecover::sInstance);
    }
}

bool KoopaBattleVs3Lv2::tryCalcAndSetBaseMtx() {
    if (isNerve(&NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvAttackRoll::sInstance) && mStateAttackRoll->tryCalcAndSetBaseMtx()) {
        return true;
    }

    return false;
}

void KoopaBattleVs3Lv2::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvAttackRoll::sInstance)) {
        if (!mStateAttackRoll->attackSensor(pSender, pReceiver)) {
            KoopaFunction::tryKoopaPushPlayer(pSender, pReceiver);
        }
    } else if (!isNerve(&NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvJumpAway::sInstance) || !KoopaFunction::tryKoopaAttackMapObj(pSender, pReceiver)) {
        if (isNerve(&NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvDamageReverse::sInstance)) {
            mStateDamageEscape->attackSensor(pSender, pReceiver);
        } else if (!KoopaFunction::tryKoopaPushPlayer(pSender, pReceiver) && KoopaFunction::tryKoopaBodyAttackPlayer(pSender, pReceiver)) {
            return;
        }
    }
}

bool KoopaBattleVs3Lv2::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvDamageReverse::sInstance)) {
        return mStateDamageEscape->tryDamage(msg, pSender, pReceiver);
    }

    if ((isNerve(&NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvAttackFire::sInstance) ||
         (isNerve(&NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvAttackRoll::sInstance) && mStateAttackRoll->isEnableGuard()) ||
         isNerve(&NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvJumpAway::sInstance) ||
         isNerve(&NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvRecover::sInstance)) &&
        mStateGuard->tryStart(msg, pSender, pReceiver)) {
        setNerve(&NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvGuard::sInstance);
        return true;
    }

    if (KoopaFunction::tryKoopaReflectStarPiece(msg, pSender, pReceiver)) {
        return true;
    }

    return false;
}

bool KoopaBattleVs3Lv2::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvDamageReverse::sInstance)) {
        return mStateDamageEscape->tryDamage(msg, pSender, pReceiver);
    }

    if (isNerve(&NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvAttackRoll::sInstance) && mStateAttackRoll->isDamage(msg, pSender, pReceiver)) {
        setNerve(&NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvDamageReverse::sInstance);
        return true;
    }

    if (isNerve(&NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvAttackFire::sInstance) && MR::isMsgToEnemyAttackBlow(msg)) {
        setNerve(&NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvDamageReverse::sInstance);
        return true;
    }

    return false;
}

void KoopaBattleVs3Lv2::exeGuard() {
    if (MR::updateActorState(this)) {
        setNerve(&NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvAttackFire::sInstance);
    }
}

void KoopaBattleVs3Lv2::exeJumpAway() {
    MR::updateActorStateAndNextNerve(this, &NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvAttackFire::sInstance);
}

void KoopaBattleVs3Lv2::exeRecover() {
    updateRecover(&NrvKoopaBattleVs3Lv2::KoopaBattleVs3Lv2NrvAttackFire::sInstance);
}

KoopaBattleVs3Lv2::~KoopaBattleVs3Lv2() {
}
