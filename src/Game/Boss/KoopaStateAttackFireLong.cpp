#include "Game/Boss/KoopaStateAttackFireLong.hpp"
#include "Game/Boss/KoopaFunction.hpp"

namespace MR {
    void moveAndTurnToPlayer(LiveActor* pActor, TVec3f* pVec, const MR::ActorMoveParam& rMoveParam) {
        MR::moveAndTurnToPlayer(pActor, pVec, rMoveParam._0, rMoveParam._4, rMoveParam._8, rMoveParam._C);
    }
}  // namespace MR

namespace {
    MR::ActorMoveParam sFallParam = {0.0f, 1.0f, 1.0f, 0.0f};
}

namespace NrvKoopaStateAttackFireLong {
    NEW_NERVE(KoopaStateAttackFireLongNrvStart, KoopaStateAttackFireLong, Start);
    NEW_NERVE(KoopaStateAttackFireLongNrvEmit, KoopaStateAttackFireLong, Emit);
}  // namespace NrvKoopaStateAttackFireLong

KoopaStateAttackFireLong::KoopaStateAttackFireLong(Koopa* pKoopa) : ActorStateBase< Koopa >("State[炎連弾攻撃]", pKoopa), _10(16), _14(-1) {
}

KoopaStateAttackFireLong::~KoopaStateAttackFireLong() {
}

void KoopaStateAttackFireLong::init() {
    initNerve(&NrvKoopaStateAttackFireLong::KoopaStateAttackFireLongNrvStart::sInstance);
}

void KoopaStateAttackFireLong::appear() {
    mIsDead = false;

    KoopaFunction::startFaceCtrl(mHost);

    _14 = 0;

    setNerve(&NrvKoopaStateAttackFireLong::KoopaStateAttackFireLongNrvStart::sInstance);
}

void KoopaStateAttackFireLong::exeStart() {
    if (MR::isFirstStep(this)) {
        MR::zeroVelocity(mHost);
        MR::startAction(mHost, "AttackFireStart");
    }

    MR::moveAndTurnToPlayer(mHost, &mHost->mFront, sFallParam);

    if (MR::isActionEnd(mHost)) {
        MR::startAction(mHost, "AttackFire");
        setNerve(&NrvKoopaStateAttackFireLong::KoopaStateAttackFireLongNrvEmit::sInstance);
    }
}

void KoopaStateAttackFireLong::exeEmit() {
    if (MR::isFirstStep(this)) {
        _14++;
    }

    MR::rotateVecDegree(KoopaFunction::getKoopaFrontPtr(mHost), mHost->mGravity, 3.0f);
    MR::moveAndTurnToPlayer(mHost, &mHost->mFront, sFallParam);

    if (MR::isStep(this, 0)) {
        KoopaFunction::emitKoopaFireLongTime(mHost);
    }

    if (MR::isStep(this, 15)) {
        if (_14 >= _10) {
            kill();
        } else {
            setNerve(&NrvKoopaStateAttackFireLong::KoopaStateAttackFireLongNrvEmit::sInstance);
        }
    }
}
