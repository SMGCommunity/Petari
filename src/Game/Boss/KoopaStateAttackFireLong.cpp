#include "Game/Boss/KoopaStateAttackFireLong.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaBattleBase.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/NerveUtil.hpp"

namespace {
    static MR::ActorMoveParam sFallParam = {0.0f, 1.0f, 1.0f, 0.0f};
    static const s32 sStepEmit = 15;
    static const s32 sEmitNumMax = 16;
    static const f32 sTurnSpeed = 3.0f;
};  // namespace

namespace NrvKoopaStateAttackFireLong {
    NEW_NERVE(KoopaStateAttackFireLongNrvStart, KoopaStateAttackFireLong, Start);
    NEW_NERVE(KoopaStateAttackFireLongNrvEmit, KoopaStateAttackFireLong, Emit);
};  // namespace NrvKoopaStateAttackFireLong

KoopaStateAttackFireLong::KoopaStateAttackFireLong(Koopa* pKoopa)
    : ActorStateBase< Koopa >("State[炎連弾攻撃]", pKoopa), mMaxFire(::sEmitNumMax), mFireEmitted(-1) {
}

void KoopaStateAttackFireLong::init() {
    initNerve(&NrvKoopaStateAttackFireLong::KoopaStateAttackFireLongNrvStart::sInstance);
}

void KoopaStateAttackFireLong::appear() {
    mIsDead = false;

    KoopaFunction::startFaceCtrl(mHost);

    mFireEmitted = 0;

    setNerve(&NrvKoopaStateAttackFireLong::KoopaStateAttackFireLongNrvStart::sInstance);
}

void KoopaStateAttackFireLong::exeStart() {
    if (MR::isFirstStep(this)) {
        MR::zeroVelocity(mHost);
        MR::startAction(mHost, "AttackFireStart");
    }

    MR::moveAndTurnToPlayer(mHost, &mHost->mFront, ::sFallParam);

    if (MR::isActionEnd(mHost)) {
        MR::startAction(mHost, "AttackFire");
        setNerve(&NrvKoopaStateAttackFireLong::KoopaStateAttackFireLongNrvEmit::sInstance);
    }
}

void KoopaStateAttackFireLong::exeEmit() {
    if (MR::isFirstStep(this)) {
        mFireEmitted++;
    }

    MR::rotateVecDegree(KoopaFunction::getKoopaFrontPtr(mHost), mHost->mGravity, ::sTurnSpeed);
    MR::moveAndTurnToPlayer(mHost, &mHost->mFront, ::sFallParam);

    if (MR::isStep(this, 0)) {
        KoopaFunction::emitKoopaFireLongTime(mHost);
    }

    if (MR::isStep(this, ::sStepEmit)) {
        if (mFireEmitted >= mMaxFire) {
            kill();
        } else {
            setNerve(&NrvKoopaStateAttackFireLong::KoopaStateAttackFireLongNrvEmit::sInstance);
        }
    }
}

KoopaStateAttackFireLong::~KoopaStateAttackFireLong() {
}
