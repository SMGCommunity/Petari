#include "Game/Boss/KoopaStateAttackSpin.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaFunction.hpp"

namespace {
    MR::ActorMoveParam sRunParam = {3.0f, 1.0f, 0.9f, 3.0f};
    MR::ActorMoveParam sParamAttackSpin1st = {2.0f, 1.0f, 0.9f, 0.0f};
    MR::ActorMoveParam sParamAttackSpin2nd = {3.0f, 1.0f, 0.9f, 0.5f};
    MR::ActorMoveParam sParamAttackSpinBrake = {1.0f, 1.0f, 0.9f, 0.0f};
    MR::ActorMoveParam sEndParam = {0.0f, 1.0f, 0.9f, 0.0f};
    MR::ActorMoveParam sEvenParam = {0.0f, 1.0f, 0.95f, 0.5f};

    void moveAndTurnToPlayer(LiveActor* pActor, TVec3f* pVec, const MR::ActorMoveParam& rMoveParam);
}  // namespace

namespace NrvKoopaStateAttackSpin {
    NEW_NERVE(KoopaStateAttackSpinNrvWaitToStart, KoopaStateAttackSpin, WaitToStart);
    NEW_NERVE(KoopaStateAttackSpinNrvRun, KoopaStateAttackSpin, Run);
    NEW_NERVE(KoopaStateAttackSpinNrvStart, KoopaStateAttackSpin, Start);
    NEW_NERVE(KoopaStateAttackSpinNrvSpin, KoopaStateAttackSpin, Spin);
    NEW_NERVE(KoopaStateAttackSpinNrvEnd, KoopaStateAttackSpin, End);
    NEW_NERVE(KoopaStateAttackSpinNrvEven, KoopaStateAttackSpin, Even);
}  // namespace NrvKoopaStateAttackSpin

KoopaStateAttackSpin::KoopaStateAttackSpin(Koopa* pKoopa)
    : ActorStateBase< Koopa >("State[スピン攻撃]", pKoopa), mMaxAttacks(1), mAttacks(), mMoveParam(&sParamAttackSpin1st), mSpinDelay(30) {
}

void KoopaStateAttackSpin::init() {
    MR::addHitSensor(mHost, "AttackSpin", ATYPE_KOOPA_ATTACK, 8, 250.0f, TVec3f(0.0f, 250.0f, 0.0f));

    initNerve(&NrvKoopaStateAttackSpin::KoopaStateAttackSpinNrvStart::sInstance);

    kill();
}

void KoopaStateAttackSpin::appear() {
    mIsDead = false;

    MR::validateHitSensor(mHost, "AttackSpin");

    mAttacks = 0;

    if (KoopaFunction::isKoopaLv1(mHost)) {
        mMaxAttacks = 1;
    } else if (KoopaFunction::isKoopaLv2(mHost)) {
        mMaxAttacks = 2;
    } else {
        mMaxAttacks = 3;
    }

    mMoveParam = &sParamAttackSpin1st;

    setNerve(&NrvKoopaStateAttackSpin::KoopaStateAttackSpinNrvWaitToStart::sInstance);
}

void KoopaStateAttackSpin::kill() {
    mIsDead = true;

    MR::invalidateHitSensor(mHost, "AttackSpin");
}

bool KoopaStateAttackSpin::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (KoopaFunction::tryKoopaPushPlayer(pSender, pReceiver)) {
        return true;
    }

    if (isNerve(&NrvKoopaStateAttackSpin::KoopaStateAttackSpinNrvSpin::sInstance) && MR::isGreaterStep(this, 5) && MR::isLessStep(this, 65) &&
        KoopaFunction::tryKoopaAttackPlayerMaximum(pSender, pReceiver)) {
        return true;
    }

    return false;
}

bool KoopaStateAttackSpin::tryDamage(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isMsgPlayerSpinAttack(msg) || !MR::isSensorPlayer(pSender)) {
        return false;
    }

    if (isNerve(&NrvKoopaStateAttackSpin::KoopaStateAttackSpinNrvEven::sInstance)) {
        return false;
    }

    MR::sendMsgEnemyAttackFlipJump(pReceiver, pSender);

    MR::tryRumblePadStrong(this, 0);

    MR::startSound(mHost, "SE_BM_KOOPA_SPIN_EVEN", -1, -1);
    MR::startSound(mHost, "SE_BV_KOOPA_GUARD", -1, -1);

    MR::stopSceneForDefaultHit(15);

    mAttacks++;
    if (mMaxAttacks >= 2 && mAttacks >= mMaxAttacks) {
        kill();
        return true;
    }

    setNerve(&NrvKoopaStateAttackSpin::KoopaStateAttackSpinNrvEven::sInstance);
    return true;
}

void KoopaStateAttackSpin::exeWaitToStart() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "Wait");
        MR::zeroVelocity(mHost);
    }

    if (MR::isStep(this, mSpinDelay)) {
        setNerve(&NrvKoopaStateAttackSpin::KoopaStateAttackSpinNrvStart::sInstance);
    }
}

void KoopaStateAttackSpin::exeRun() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "Run");
    }

    if (KoopaFunction::moveAndTurnKoopaToPlayer(mHost, sRunParam)) {
        return;
    }

    if (MR::isBindedWall(mHost)) {
        MR::addVelocityJump(mHost, 10.0f);
    }

    if (MR::isNearPlayer(mHost, 1000.0f) || MR::isGreaterStep(this, 120)) {
        if (MR::calcGravitySpeed(mHost) >= 0.0f) {
            setNerve(&NrvKoopaStateAttackSpin::KoopaStateAttackSpinNrvStart::sInstance);
        }
    }
}

void KoopaStateAttackSpin::exeStart() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "SpinStart");
        MR::zeroVelocity(mHost);
    }

    MR::turnDirectionToPlayerDegree(mHost, &mHost->mFront, 5.0f);

    if (MR::isActionEnd(mHost)) {
        setNerve(&NrvKoopaStateAttackSpin::KoopaStateAttackSpinNrvSpin::sInstance);
    }
}

void KoopaStateAttackSpin::exeSpin() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "Spin");
        MR::zeroVelocity(mHost);
    }

    if (MR::isLessStep(this, 60)) {
        KoopaFunction::moveAndTurnKoopaToPlayer(mHost, *mMoveParam);
    } else {
        KoopaFunction::moveAndTurnKoopaToPlayer(mHost, sParamAttackSpinBrake);
    }

    if (MR::isBindedWall(mHost)) {
        MR::addVelocityJump(mHost, 15.0f);
    }

    if (MR::isActionEnd(mHost)) {
        mAttacks++;

        if (!MR::isPlayerDamaging() && mAttacks < mMaxAttacks) {
            mMoveParam = &sParamAttackSpin2nd;
            setNerve(&NrvKoopaStateAttackSpin::KoopaStateAttackSpinNrvStart::sInstance);
        } else {
            setNerve(&NrvKoopaStateAttackSpin::KoopaStateAttackSpinNrvEnd::sInstance);
        }
    }
}

void KoopaStateAttackSpin::exeEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "SpinEnd");
    }

    KoopaFunction::moveAndTurnKoopaToPlayer(mHost, sEndParam);

    if (MR::isActionEnd(mHost)) {
        MR::zeroVelocity(mHost);
        kill();
    }
}

void KoopaStateAttackSpin::exeEven() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "SpinEven");
        MR::calcVecFromPlayerH(&mHost->mVelocity, mHost);
        MR::scaleVelocity(mHost, 30.0f);
    }

    KoopaFunction::moveAndTurnKoopaToPlayer(mHost, sEvenParam);

    if (MR::isActionEnd(mHost)) {
        if (!MR::isPlayerDamaging() && mAttacks < mMaxAttacks) {
            mMoveParam = &sParamAttackSpin2nd;
            setNerve(&NrvKoopaStateAttackSpin::KoopaStateAttackSpinNrvSpin::sInstance);
        } else {
            kill();
        }
    }
}

KoopaStateAttackSpin::~KoopaStateAttackSpin() {
}
