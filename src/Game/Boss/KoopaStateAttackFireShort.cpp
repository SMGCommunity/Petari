#include "Game/Boss/KoopaStateAttackFireShort.hpp"
#include "Game/Boss/KoopaFunction.hpp"

namespace {
    MR::ActorMoveParam sFallParam = {0.0f, 1.0f, 1.0f, 0.0f};
}  // namespace

namespace MR {
    void moveAndTurnToPlayer(LiveActor* pActor, TVec3f* pVec, const MR::ActorMoveParam& rMoveParam) {
        MR::moveAndTurnToPlayer(pActor, pVec, rMoveParam._0, rMoveParam._4, rMoveParam._8, rMoveParam._C);
    }
}  // namespace MR

namespace NrvKoopaStateAttackFireShort {
    NEW_NERVE(KoopaStateAttackFireShortNrvStart, KoopaStateAttackFireShort, Start);
    NEW_NERVE(KoopaStateAttackFireShortNrvEmit, KoopaStateAttackFireShort, Emit);
}  // namespace NrvKoopaStateAttackFireShort

KoopaStateAttackFireShort::KoopaStateAttackFireShort(Koopa* pKoopa)
    : ActorStateBase< Koopa >("State[ショート炎攻撃]", pKoopa), _10(-1), _14(3), _18(45) {
}

KoopaStateAttackFireShort::~KoopaStateAttackFireShort() {
}

void KoopaStateAttackFireShort::init() {
    initNerve(&NrvKoopaStateAttackFireShort::KoopaStateAttackFireShortNrvEmit::sInstance);
}

void KoopaStateAttackFireShort::appear() {
    mIsDead = false;

    KoopaFunction::startFaceCtrl(mHost);

    _10 = 0;

    if (KoopaFunction::isKoopaVs1(mHost)) {
        KoopaFunction::isKoopaLv2(mHost) ? _14 = 3 : _14 = 5;

        _18 = 45;
    } else if (KoopaFunction::isKoopaVs2(mHost)) {
        _14 = 5;
        _18 = 30;
    } else if (KoopaFunction::isKoopaLv2(mHost)) {
        if (KoopaFunction::isKoopaAngry(mHost)) {
            _14 = 5;
            _18 = 15;
        } else {
            _14 = 3;
            _18 = 30;
        }
    } else if (KoopaFunction::isKoopaLv3(mHost)) {
        KoopaFunction::isKoopaAngry(mHost) ? _14 = 10 : _14 = 5;
        _18 = 15;
    } else {
        _14 = 3;
        _18 = 30;
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
        _10++;
    }
    Koopa* pKoopa = mHost;
    MR::turnDirectionToPlayerDegree(pKoopa, KoopaFunction::getKoopaFrontPtr(pKoopa), 2.0f);
    MR::moveAndTurnToPlayer(mHost, &mHost->mFront, sFallParam);

    if (MR::isStep(this, 1)) {
        if (KoopaFunction::isKoopaVs1(mHost) && !KoopaFunction::isKoopaLv3(mHost)) {
            KoopaFunction::emitKoopaFireShortSlow(mHost);
        } else {
            KoopaFunction::isKoopaVs2(mHost) ? KoopaFunction::emitKoopaFireShortCurve(mHost) : KoopaFunction::emitKoopaFireShortFast(mHost);
        }
    }

    if (_10 < _14) {
        if (MR::isStep(this, _18)) {
            setNerve(&NrvKoopaStateAttackFireShort::KoopaStateAttackFireShortNrvEmit::sInstance);
        }
    } else {
        if (MR::isGreaterStep(this, 30)) {
            kill();
        }
    }
}
