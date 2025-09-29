#include "Game/Enemy/Karon.hpp"
#include "Game/Enemy/TerritoryMover.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/Binder.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"

namespace {
    static const KaronParam hNoMoveNoTurnParam = { 0.0f, 1.2f, 0.80000001f, 0.0f };
    static const KaronParam hNoMoveNoTurnOnAirParam = { 0.0f, 2.0f, 0.94f, 0.0f };
    static const KaronParam hWalkparam = { 0.12f, 1.2f, 0.94f, 2.0f };
    static const KaronParam hSearchParam = { 0.0f, 1.2f, 0.94f, 2.0f };
    static const KaronParam hPursueParam = { 0.25f, 1.2f, 0.94f, 2.0f };
    static const KaronParam hHitReactionOnGroundParam = { 0.0f, 1.2f, 0.80f, 5.0f };
    static const KaronParam hHitReactionAirParam = { 0.0f, 2.0f, 0.94f, 5.0f };
};

namespace NrvKaron {
    NEW_NERVE(HostTypeNrvFixWait, Karon, FixWait);
    NEW_NERVE(HostTypeNrvWait, Karon, Wait);
    NEW_NERVE(HostTypeNrvStarPieceHitWait, Karon, Wait);
    NEW_NERVE(HostTypeNrvWalk, Karon, Walk);
    NEW_NERVE(HostTypeNrvTurn, Karon, Turn);
    NEW_NERVE(HostTypeNrvWalkOnRail, Karon, WalkOnRail);
    NEW_NERVE(HostTypeNrvSearch, Karon, Search);
    NEW_NERVE_ONEND(HostTypeNrvPursue, Karon, Pursue, Pursue);
    NEW_NERVE(HostTypeNrvBroken, Karon, Broken);
    NEW_NERVE(HostTypeNrvStepBroken, Karon, StepBroken);
    NEW_NERVE(HostTypeNrvRecover, Karon, Recover);
    NEW_NERVE(HostTypeNrvHitReaction, Karon, HitReaction);
    NEW_NERVE(HostTypeNrvStarPieceHit, Karon, HitReaction);
    NEW_NERVE(HostTypeNrvDeath, Karon, Death);
    NEW_NERVE(HostTypeNrvSinkDown, Karon, SinkDown);
    NEW_NERVE_ONEND(HostTypeNrvBindStarPointer, Karon, BindStarPointer, BindStarPointer);
};

Karon::Karon(const char *pName) : LiveActor(pName) {
    mTerritoryMover = nullptr;
    _90 = 0;
    _94 = 0;
    mScaleController = nullptr;
    mStateStarPointer = nullptr;
}

void Karon::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    MR::useStageSwitchWriteDead(this, rIter);
    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
    }

    bool needsFixWait = false;
    if(MR::useStageSwitchReadA(this, rIter)) {
        needsFixWait = true;
    }

    if (MR::isConnectedWithRail(rIter)) {
        initRailRider(rIter);
    }

    s32 arg0 = -1;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);

    if (arg0 == -1) {
        _94 = 0;
    }
    else {
        _94 = 1;
    }

    initModelManagerWithAnm("Karon", nullptr, false);
    MR::connectToSceneEnemy(this);
    initBinder(70.0f, 70.0f, 0);
    MR::onCalcGravity(this);
    initSound(4, false);
    initEffectKeeper(1, nullptr, false);

    if (needsFixWait) {
        initNerve(&NrvKaron::HostTypeNrvFixWait::sInstance);
    }
    else {
        initNerve(&NrvKaron::HostTypeNrvWait::sInstance);
    }

    initHitSensor(1);
    f32 y_scale = (90.0f * mScale.y);
    TVec3f v13;
    v13.x = 0.0f;
    v13.y = y_scale;
    v13.z = 0.0f;
    MR::addHitSensorEnemy(this, "body", 16, y_scale, v13);
    MR::initShadowVolumeSphere(this, 60.0f);
    MR::initLightCtrl(this);
    MR::declareStarPiece(this, 3);
    MR::validateClipping(this);
    MR::setGroupClipping(this, rIter, 32);
    mTerritoryMover = new TerritoryMover(500.0f);
    mTerritoryMover->_4.x = mPosition.x;
    mTerritoryMover->_4.y = mPosition.y;
    mTerritoryMover->_4.z = mPosition.z;
    TVec3f targetPos(0.0f, 50.0f, 0.0f);
    MR::initStarPointerTarget(this, 100.0f, targetPos);
    mScaleController = new AnimScaleController(0);
    mStateStarPointer = new WalkerStateBindStarPointer(this, mScaleController);
    makeActorAppeared();
}

void Karon::initAfterPlacement() {
    MR::trySetMoveLimitCollision(this);
}

void Karon::kill() {
    LiveActor::kill();

    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }

    MR::emitEffect(this, "Death");
    MR::startSoundSeVer(this, "SE_EM_EXPLODE_S", -1, -1);
}

void Karon::exeFixWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Wait");
        if (!_94) {
            MR::stopBck(this);
        }
    }

    mBinder->_1EC._5 = true;
    MR::moveAndTurnToPlayer(this, hNoMoveNoTurnParam._0, hNoMoveNoTurnParam._4, hNoMoveNoTurnParam._8, hNoMoveNoTurnParam._C);

    if (MR::isOnSwitchA(this)) {
        setNerve(&NrvKaron::HostTypeNrvWait::sInstance);
    }
}

void Karon::exeStepBroken() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "StepBreak");
        MR::startSound(this, "SE_EM_KARON_BROKEN", -1, -1);
        MR::startSound(this, "SE_EV_KARON_DAMAGE", -1, -1);
        MR::offCalcGravity(this);
        MR::forceBindOnGround(this, 0.0f, 1.0f);
    }

    TVec3f v6(0, 0, 0);
    if (MR::calcVelocityAreaOrRailMoveOnGround(&v6, this)) {
        MR::addVelocityLimit(this, v6 * 1.0f);
        MR::moveAndTurnToPlayer(this, hNoMoveNoTurnParam._0, hNoMoveNoTurnParam._4, hNoMoveNoTurnParam._8, hNoMoveNoTurnParam._C);
        MR::calcGravity(this);
    }
    else if (MR::isBinded(this)) {
        TVec3f* bindNormal = &MR::getBindedNormal(this);
        mVelocity.scale(bindNormal->dot(mVelocity));
    }
    else {
        TVec3f* g = &mGravity;
        mVelocity.scale(g->dot(mVelocity), mGravity);
        MR::forceBindOnGround(this, 1.0f, 0.8f);
    }

    mBinder->_1EC._5 = true;
    if (MR::isGreaterStep(this, 300)) {
        setNerve(&NrvKaron::HostTypeNrvRecover::sInstance);
    }
}

void Karon::exeRecover() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Recovery");
        MR::startSound(this, "SE_EM_KARON_RECOVER", -1, -1);
    }

    if (MR::isOnGround(this)) {
        TVec3f* g = &mGravity;
        mVelocity.scale(g->dot(mVelocity), *g);
    }

    MR::moveAndTurnToPlayer(this, hNoMoveNoTurnParam._0, hNoMoveNoTurnParam._4, hNoMoveNoTurnParam._8, hNoMoveNoTurnParam._C);
    mBinder->_1EC._5 = true;
    if (MR::isActionEnd(this)) {
        MR::onCalcGravity(this);
        setNerve(&NrvKaron::HostTypeNrvWait::sInstance);
    }

}

void Karon::exeHitReaction() {
    if (MR::isFirstStep(this)) {
        if (isNerve(&NrvKaron::HostTypeNrvStarPieceHit::sInstance)) {
            MR::startAction(this, "StarPiece");
        }
        else {
            MR::startAction(this, "AttackHit");
        }
    }

    MR::startLevelSound(this, "SE_EV_LV_KARON_ATK_SUCCESS", -1, -1, -1);

    if (isNerve(&NrvKaron::HostTypeNrvStarPieceHit::sInstance)) {
        if (MR::isOnGround(this)) {
            MR::moveAndTurnToPlayer(this, hNoMoveNoTurnParam._0, hNoMoveNoTurnParam._4, hNoMoveNoTurnParam._8, hNoMoveNoTurnParam._C);
        }
        else {
            MR::moveAndTurnToPlayer(this, hNoMoveNoTurnOnAirParam._0, hNoMoveNoTurnOnAirParam._4, hNoMoveNoTurnOnAirParam._8, hNoMoveNoTurnOnAirParam._C);
        }
    }
    else if (MR::isOnGround(this)) {
        MR::moveAndTurnToPlayer(this, hHitReactionOnGroundParam._0, hHitReactionOnGroundParam._4, hHitReactionOnGroundParam._8, hHitReactionOnGroundParam._C);
    }
    else {
        MR::moveAndTurnToPlayer(this, hHitReactionAirParam._0, hHitReactionAirParam._4, hHitReactionAirParam._8, hHitReactionAirParam._C);
    }

    if (MR::isOnGround(this) && MR::isActionEnd(this)) {
        if (isNerve(&NrvKaron::HostTypeNrvStarPieceHit::sInstance)) {
            setNerve(&NrvKaron::HostTypeNrvStarPieceHitWait::sInstance);
        }
        else {
            setNerve(&NrvKaron::HostTypeNrvWait::sInstance);
        }
    }    
}

void Karon::exeDeath() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Down");
        MR::startSound(this, "SE_EM_KARON_BROKEN", -1, -1);
        MR::startSound(this, "SE_EV_KARON_DAMAGE", -1, -1);
    }

    moveAndTurn(&hNoMoveNoTurnParam);
    
    if (MR::isActionEnd(this)) {
        kill();
    }
}

void Karon::endBindStarPointer() {
    mStateStarPointer->kill();
}
