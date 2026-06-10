#include "Game/Boss/KoopaStateAttackFireShort.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaFunction.hpp"

namespace {
    MR::ActorMoveParam sFallParam = {0.0f, 1.0f, 1.0f, 0.0f};
};  // namespace

namespace MR {
    void moveAndTurnToPlayer(LiveActor* pActor, TVec3f* pVec, const MR::ActorMoveParam& rMoveParam);
};  // namespace MR

namespace NrvKoopaStateAttackFireShort {
    NEW_NERVE(KoopaStateAttackFireShortNrvStart, KoopaStateAttackFireShort, Start);
    NEW_NERVE(KoopaStateAttackFireShortNrvEmit, KoopaStateAttackFireShort, Emit);
};  // namespace NrvKoopaStateAttackFireShort

KoopaStateAttackFireShort::KoopaStateAttackFireShort(Koopa* pKoopa)
    : ActorStateBase< Koopa >("State[ショート炎攻撃]", pKoopa), mFireEmitted(-1), mMaxFire(3), mFireDelay(45) {
}

void KoopaStateAttackFireShort::init() {
    initNerve(&NrvKoopaStateAttackFireShort::KoopaStateAttackFireShortNrvStart::sInstance);
}

void KoopaStateAttackFireShort::appear() {
    mIsDead = false;

    KoopaFunction::startFaceCtrl(mHost);

    mFireEmitted = 0;

    if (KoopaFunction::isKoopaVs1(mHost)) {
        KoopaFunction::isKoopaLv2(mHost) ? mMaxFire = 3 : mMaxFire = 5;

        mFireDelay = 45;
    } else if (KoopaFunction::isKoopaVs2(mHost)) {
        mMaxFire = 5;
        mFireDelay = 30;
    } else if (KoopaFunction::isKoopaLv2(mHost)) {
        if (KoopaFunction::isKoopaAngry(mHost)) {
            mMaxFire = 5;
            mFireDelay = 15;
        } else {
            mMaxFire = 3;
            mFireDelay = 30;
        }
    } else if (KoopaFunction::isKoopaLv3(mHost)) {
        KoopaFunction::isKoopaAngry(mHost) ? mMaxFire = 10 : mMaxFire = 5;
        mFireDelay = 15;
    } else {
        mMaxFire = 3;
        mFireDelay = 30;
    }

    setNerve(&NrvKoopaStateAttackFireShort::KoopaStateAttackFireShortNrvStart::sInstance);
}

void KoopaStateAttackFireShort::exeStart() {
    if (MR::isFirstStep(this)) {
        MR::zeroVelocity(mHost);
        MR::startAction(mHost, "AttackFireStart");
    }

    MR::moveAndTurnToPlayer(mHost, &mHost->mFront, sFallParam);

    if (MR::isActionEnd(mHost)) {
        MR::startAction(mHost, "AttackFire");
        setNerve(&NrvKoopaStateAttackFireShort::KoopaStateAttackFireShortNrvEmit::sInstance);
    }
}

void KoopaStateAttackFireShort::exeEmit() {
    if (MR::isFirstStep(this)) {
        mFireEmitted++;
    }

    Koopa* pKoopa = mHost;
    MR::turnDirectionToPlayerDegree(pKoopa, KoopaFunction::getKoopaFrontPtr(pKoopa), 2.0f);
    MR::moveAndTurnToPlayer(mHost, &mHost->mFront, sFallParam);

    if (MR::isStep(this, 1)) {
        if (KoopaFunction::isKoopaVs1(mHost) && !KoopaFunction::isKoopaLv3(mHost)) {
            KoopaFunction::emitKoopaFireShortSlow(mHost);
        } else if (KoopaFunction::isKoopaVs2(mHost)) {
            KoopaFunction::emitKoopaFireShortCurve(mHost);
        } else {
            KoopaFunction::emitKoopaFireShortFast(mHost);
        }
    }

    if (mFireEmitted < mMaxFire) {
        if (MR::isStep(this, mFireDelay)) {
            setNerve(&NrvKoopaStateAttackFireShort::KoopaStateAttackFireShortNrvEmit::sInstance);
        }
    } else {
        if (MR::isGreaterStep(this, 30)) {
            kill();
        }
    }
}

KoopaStateAttackFireShort::~KoopaStateAttackFireShort() {
}
