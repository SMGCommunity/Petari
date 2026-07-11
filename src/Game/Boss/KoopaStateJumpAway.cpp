#include "Game/Boss/KoopaStateJumpAway.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static const f32 sJumpFrontSpeed = 20.0f;
    static const f32 sJumpFlyUpSpeed = 35.0f;
    static const f32 sJumpGravity = 1.0f;
    static const s32 sLandStepVs3Lv3 = 30;
};  // namespace

namespace NrvKoopaStateJumpAway {
    NEW_NERVE(KoopaStateJumpAwayNrvJumpStart, KoopaStateJumpAway, JumpStart);
    NEW_NERVE(KoopaStateJumpAwayNrvJump, KoopaStateJumpAway, Jump);
    NEW_NERVE(KoopaStateJumpAwayNrvLand, KoopaStateJumpAway, Land);
};  // namespace NrvKoopaStateJumpAway

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

        MR::setVelocity(pKoopa, KoopaFunction::getKoopaFront(pKoopa) * -::sJumpFrontSpeed);
        MR::addVelocityJump(mHost, ::sJumpFlyUpSpeed);

        MR::startSound(mHost, "SE_BM_KOOPA_JUMP");
        MR::startSound(mHost, "SE_BV_KOOPA_JUMP");
    }

    MR::addVelocityToGravity(mHost, ::sJumpGravity);
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
        MR::startSound(mHost, "SE_BM_KOOPA_LAND");

        MR::zeroVelocity(mHost);

        setNerve(&NrvKoopaStateJumpAway::KoopaStateJumpAwayNrvLand::sInstance);
    }
}

void KoopaStateJumpAway::exeLand() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "JumpAwayLand");
    }

    if (KoopaFunction::isKoopaVs3(mHost) && KoopaFunction::isKoopaLv3(mHost) && MR::isGreaterStep(this, ::sLandStepVs3Lv3)) {
        kill();
    } else if (MR::isActionEnd(mHost)) {
        kill();
    }
}

KoopaStateJumpAway::~KoopaStateJumpAway() {
}
