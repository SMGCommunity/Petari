#include "Game/Boss/KoopaStateJumpAway.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaFunction.hpp"

namespace NrvKoopaStateJumpAway {
    NEW_NERVE(KoopaStateJumpAwayNrvJumpStart, KoopaStateJumpAway, JumpStart);
    NEW_NERVE(KoopaStateJumpAwayNrvJump, KoopaStateJumpAway, Jump);
    NEW_NERVE(KoopaStateJumpAwayNrvLand, KoopaStateJumpAway, Land);
}  // namespace NrvKoopaStateJumpAway

KoopaStateJumpAway::KoopaStateJumpAway(Koopa* pKoopa) : ActorStateBase< Koopa >("State[ジャンプで離れる]", pKoopa) {
}

void KoopaStateJumpAway::init() {
    initNerve(&NrvKoopaStateJumpAway::KoopaStateJumpAwayNrvJumpStart::sInstance);

    kill();
}

void KoopaStateJumpAway::appear() {
    mIsDead = false;

    setNerve(&NrvKoopaStateJumpAway::KoopaStateJumpAwayNrvJumpStart::sInstance);
}

void KoopaStateJumpAway::exeJumpStart() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "JumpAwayStart");

        MR::zeroVelocity(mHost);
    }

    if (MR::isActionEnd(mHost)) {
        setNerve(&NrvKoopaStateJumpAway::KoopaStateJumpAwayNrvJump::sInstance);
    }
}

void KoopaStateJumpAway::exeJump() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "JumpAway");

        Koopa* pKoopa = mHost;
        TVec3f front = KoopaFunction::getKoopaFront(pKoopa);
        front.mult(-20.0f);

        MR::setVelocity(pKoopa, front);
        MR::addVelocityJump(mHost, 35.0f);

        MR::startSound(mHost, "SE_BM_KOOPA_JUMP", -1, -1);
        MR::startSound(mHost, "SE_BV_KOOPA_JUMP", -1, -1);
    }

    MR::addVelocityToGravity(mHost, 1.0f);
    TVec3f newFront = mHost->mVelocity;
    MR::vecKillElement(newFront, mHost->mGravity, &newFront);

    if (!MR::isNearZero(newFront)) {
        MR::normalize(&newFront);

        KoopaFunction::getKoopaFrontPtr(mHost)->set(newFront.negateInline());
    }

    Koopa* pKoopa = mHost;
    if (!MR::sendMsgEnemyAttackToBindedSensor(pKoopa, pKoopa->getSensor("Body")) && !MR::isFirstStep(this) &&
        mHost->mVelocity.dot(mHost->mGravity) >= 0.0f && MR::isBindedGround(mHost)) {
        MR::tryRumblePadAndCameraDistanceStrong(mHost, 1500.0f, 2000.0f, 2000.0f);
        MR::startSound(mHost, "SE_BM_KOOPA_LAND", -1, -1);

        MR::zeroVelocity(mHost);

        setNerve(&NrvKoopaStateJumpAway::KoopaStateJumpAwayNrvLand::sInstance);
    }
}

void KoopaStateJumpAway::exeLand() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "JumpAwayLand");
    }

    if (KoopaFunction::isKoopaVs3(mHost) && KoopaFunction::isKoopaLv3(mHost) && MR::isGreaterStep(this, 30)) {
        kill();
    } else if (MR::isActionEnd(mHost)) {
        kill();
    }
}

KoopaStateJumpAway::~KoopaStateJumpAway() {
}
