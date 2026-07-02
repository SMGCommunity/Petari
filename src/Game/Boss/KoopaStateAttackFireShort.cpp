#include "Game/Boss/KoopaStateAttackFireShort.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaBattleBase.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/NerveUtil.hpp"

namespace {
    static MR::ActorMoveParam sFallParam = {0.0f, 1.0f, 1.0f, 0.0f};
    // static const s32 sStepEmit = _;
    static const s32 sStepToEmitFire = 1;
    static const s32 sStepToNextAttack = 45;
    static const s32 sStepToNextAttackVs2 = 30;
    static const s32 sStepToNextAttackVs3 = 30;
    static const s32 sStepToNextAttackVs3Angry = 15;
    // static const s32 sFireAttackStep = _;
    static const f32 sTurnSpeed = 2.0f;
};  // namespace

namespace NrvKoopaStateAttackFireShort {
    NEW_NERVE(KoopaStateAttackFireShortNrvStart, KoopaStateAttackFireShort, Start);
    NEW_NERVE(KoopaStateAttackFireShortNrvEmit, KoopaStateAttackFireShort, Emit);
};  // namespace NrvKoopaStateAttackFireShort

KoopaStateAttackFireShort::KoopaStateAttackFireShort(Koopa* pKoopa)
    : ActorStateBase< Koopa >("State[ショート炎攻撃]", pKoopa), mFireEmitted(-1), mMaxFire(3), mFireDelay(::sStepToNextAttack) {
}

void KoopaStateAttackFireShort::init() {
    initNerve(&NrvKoopaStateAttackFireShort::KoopaStateAttackFireShortNrvStart::sInstance);
}

void KoopaStateAttackFireShort::appear() {
    mIsDead = false;

    KoopaFunction::startFaceCtrl(mHost);

    mFireEmitted = 0;

    if (KoopaFunction::isKoopaVs1(mHost)) {
        if (KoopaFunction::isKoopaLv2(mHost)) {
            mMaxFire = 3;
        } else {
            mMaxFire = 5;
        }

        mFireDelay = ::sStepToNextAttack;
    } else if (KoopaFunction::isKoopaVs2(mHost)) {
        mMaxFire = 5;
        mFireDelay = ::sStepToNextAttackVs2;
    } else {
        if (KoopaFunction::isKoopaLv2(mHost)) {
            if (KoopaFunction::isKoopaAngry(mHost)) {
                mMaxFire = 5;
                mFireDelay = ::sStepToNextAttackVs3Angry;
            } else {
                mMaxFire = 3;
                mFireDelay = ::sStepToNextAttackVs3;
            }
        } else {
            if (KoopaFunction::isKoopaLv3(mHost)) {
                if (KoopaFunction::isKoopaAngry(mHost)) {
                    mMaxFire = 10;
                } else {
                    mMaxFire = 5;
                }

                mFireDelay = ::sStepToNextAttackVs3Angry;
            } else {
                mMaxFire = 3;
                mFireDelay = ::sStepToNextAttackVs3;
            }
        }
    }

    setNerve(&NrvKoopaStateAttackFireShort::KoopaStateAttackFireShortNrvStart::sInstance);
}

void KoopaStateAttackFireShort::exeStart() {
    if (MR::isFirstStep(this)) {
        MR::zeroVelocity(mHost);
        MR::startAction(mHost, "AttackFireStart");
    }

    MR::moveAndTurnToPlayer(mHost, &mHost->mFront, ::sFallParam);

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
    MR::turnDirectionToPlayerDegree(pKoopa, KoopaFunction::getKoopaFrontPtr(pKoopa), ::sTurnSpeed);
    MR::moveAndTurnToPlayer(mHost, &mHost->mFront, ::sFallParam);

    if (MR::isStep(this, ::sStepToEmitFire)) {
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
    } else if (MR::isGreaterStep(this, 30)) {
        kill();
    }
}

KoopaStateAttackFireShort::~KoopaStateAttackFireShort() {
}
