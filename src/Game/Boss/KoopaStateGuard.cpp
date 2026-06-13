#include "Game/Boss/KoopaStateGuard.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaBattleBase.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    MR::ActorMoveParam sGuardFallParam = {0.0f, 1.0f, 0.98f, 0.0f};
};  // namespace

namespace NrvKoopaStateGuard {
    NEW_NERVE(KoopaStateGuardNrvGuardFace, KoopaStateGuard, GuardFace);
    NEW_NERVE(KoopaStateGuardNrvGuardBody, KoopaStateGuard, GuardBody);
    NEW_NERVE(KoopaStateGuardNrvGuardTail, KoopaStateGuard, GuardTail);
};  // namespace NrvKoopaStateGuard

KoopaStateGuard::KoopaStateGuard(Koopa* pKoopa) : ActorStateBase< Koopa >("State[ガード攻撃]", pKoopa) {
}

void KoopaStateGuard::init() {
    initNerve(&NrvKoopaStateGuard::KoopaStateGuardNrvGuardFace::sInstance);

    kill();
}

bool KoopaStateGuard::tryStart(u32 msg, HitSensor* pReceiver, HitSensor* pSender) {
    if (!MR::isSensorPlayer(pReceiver) || !MR::isMsgPlayerSpinAttack(msg)) {
        return false;
    }

    if (MR::isSensor(pSender, "Face")) {
        MR::sendMsgEnemyAttackFlip(pReceiver, pSender);

        setNerve(&NrvKoopaStateGuard::KoopaStateGuardNrvGuardFace::sInstance);

        return true;
    }

    if (MR::isSensor(pSender, "GuardBody")) {
        MR::sendMsgEnemyAttackFlip(pReceiver, pSender);

        setNerve(&NrvKoopaStateGuard::KoopaStateGuardNrvGuardBody::sInstance);

        return true;
    }

    if (MR::isSensor(pSender, "Tail") || MR::isSensor(pSender, "TailTop")) {
        MR::sendMsgEnemyAttackFlipMaximum(pReceiver, pSender);

        setNerve(&NrvKoopaStateGuard::KoopaStateGuardNrvGuardTail::sInstance);

        return true;
    }

    return false;
}

void KoopaStateGuard::exeGuardFace() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "GuardFace");
        MR::startSound(mHost, "SE_BV_KOOPA_GUARD");

        MR::zeroVelocity(mHost);
    }

    Koopa* pKoopa = mHost;
    MR::moveAndTurnToPlayer(pKoopa, KoopaFunction::getKoopaFrontPtr(pKoopa), ::sGuardFallParam);

    if (MR::isActionEnd(mHost)) {
        kill();
    }
}

void KoopaStateGuard::exeGuardBody() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "GuardBody");
        MR::startSound(mHost, "SE_BV_KOOPA_GUARD");

        MR::zeroVelocity(mHost);
    }

    Koopa* pKoopa = mHost;
    MR::moveAndTurnToPlayer(pKoopa, KoopaFunction::getKoopaFrontPtr(pKoopa), ::sGuardFallParam);

    if (MR::isActionEnd(mHost)) {
        kill();
    }
}

void KoopaStateGuard::exeGuardTail() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "AttackTail");

        MR::zeroVelocity(mHost);
    }

    Koopa* pKoopa = mHost;
    MR::moveAndTurnToPlayer(pKoopa, KoopaFunction::getKoopaFrontPtr(pKoopa), ::sGuardFallParam);

    if (MR::isActionEnd(mHost)) {
        kill();
    }
}

KoopaStateGuard::~KoopaStateGuard() {
}
