#include "Game/Boss/KoopaBattleBase.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/Boss/KoopaStateDamageEscape.hpp"
#include "Game/Boss/KoopaStateGuard.hpp"

namespace {
    MR::ActorMoveParam sFindParam = {0.0f, 1.0f, 0.98f, 3.0f};
}  // namespace

KoopaBattleBase::KoopaBattleBase(const char* pName, Koopa* pKoopa)
    : ActorStateBase< Koopa >(pName, pKoopa), mStateDamageEscape(), mStateGuard(), mWanderTime(-1) {
}

void KoopaBattleBase::init() {
    mStateDamageEscape = new KoopaStateDamageEscape(mHost);
    mStateGuard = new KoopaStateGuard(mHost);
}

void KoopaBattleBase::kill() {
    mIsDead = true;

    MR::zeroVelocity(mHost);

    KoopaFunction::startRecoverKoopaArmor(mHost);
    KoopaFunction::startRecoverKoopaTailThorn(mHost);

    MR::overlayWithPreviousScreen(2);
}

void KoopaBattleBase::updateChasePlayer(const MR::ActorMoveParam& rMoveParam) {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "Walk");
        MR::onBind(mHost);
        KoopaFunction::startFaceCtrl(mHost);
    }

    if (MR::isBindedWall(mHost)) {
        Koopa* pKoopa = mHost;
        if (!MR::sendMsgEnemyAttackToBindedSensor(pKoopa, pKoopa->getSensor("Body"))) {
            MR::addVelocityJump(mHost, 15.0f);
        }
    }

    MR::moveAndTurnToPlayer(mHost, &mHost->mFront, rMoveParam);

    Koopa* pKoopa = mHost;
    MR::setBckRate(pKoopa, MR::calcVelocityLength(pKoopa) * 0.18f);
}

bool KoopaBattleBase::updateWander(const MR::ActorMoveParam& rMoveParam) {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "Walk");

        MR::onBind(mHost);

        KoopaFunction::endFaceCtrl(mHost, -1);

        mWanderTime = MR::getRandom(60l, 120l);
    }

    if (MR::isBindedWall(mHost)) {
        Koopa* pKoopa = mHost;
        if (!MR::sendMsgEnemyAttackToBindedSensor(pKoopa, pKoopa->getSensor("Body"))) {
            MR::addVelocityJump(mHost, 15.0f);
        }
    }

    MR::moveAndTurnToDirection(mHost, KoopaFunction::getKoopaFrontPtr(mHost), KoopaFunction::getKoopaFront(mHost), rMoveParam._0, rMoveParam._4,
                               rMoveParam._8, rMoveParam._C);

    Koopa* pKoopa = mHost;
    MR::setBckRate(pKoopa, MR::calcVelocityLength(pKoopa) * 0.18f);

    return MR::isStep(this, mWanderTime);
}

bool KoopaBattleBase::updateSearch() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "Search");
        MR::zeroVelocity(mHost);
    }

    MR::rotateVecDegree(KoopaFunction::getKoopaFrontPtr(mHost), mHost->mGravity, 2.4f);

    if (MR::isStep(this, 120)) {
        return true;
    }

    return false;
}

bool KoopaBattleBase::updateFind() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "Find");
        MR::setVelocityJump(mHost, 10.0f);
    }

    Koopa* pKoopa = mHost;
    MR::moveAndTurnToPlayer(pKoopa, KoopaFunction::getKoopaFrontPtr(pKoopa), sFindParam);

    if (!MR::isFirstStep(this) && MR::isBindedGround(mHost)) {
        MR::tryRumblePadAndCameraDistanceMiddle(mHost, 800.0f, 1200.0f, 2000.0f);
        return true;
    }

    return false;
}

bool KoopaBattleBase::updateRecover(const Nerve* pNerve) {
    if (MR::isFirstStep(this)) {
        MR::zeroVelocity(mHost);
        MR::startAction(mHost, "Recover");
        KoopaFunction::startRecoverKoopaArmor(mHost);
    }

    if (MR::isActionEnd(mHost)) {
        setNerve(pNerve);

        return true;
    }

    return false;
}

KoopaBattleBase::~KoopaBattleBase() {
}

namespace MR {
    void moveAndTurnToPlayer(LiveActor* pActor, TVec3f* pVec, const MR::ActorMoveParam& rMoveParam) {
        moveAndTurnToPlayer(pActor, pVec, rMoveParam._0, rMoveParam._4, rMoveParam._8, rMoveParam._C);
    }
}  // namespace MR
