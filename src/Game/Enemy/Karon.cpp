#include "Game/Enemy/Karon.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/TerritoryMover.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/Binder.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

void Karon_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)2.0f;
}

namespace {
    // hGravity
    // hTurnLimitRadian
    // hUpdateUpRate
    // hWallJumpVel
    // hWallJumpJudge
    // hPushRate
    // hPushMaxVel
    // hFaceToDegree
    // hForceBindOnGroundOffset
    // hForceBindOnGroundMul
    // hFrontLength
    // hHeight
    // hLimit
    // hShiftingSandVel
    // hPursueDistance
    // hFireAttackDistance
    // hFollowRailDistance
    // hToRailDiv
    // hWaitTime
    // hStarPieceHitWaitTime
    // hWalkLimitTime
    // hGoalDist
    // hPrePursueJumpVel
    // hHitReactionTime
    // hHitReactionVelH
    // hHitReactionVelV
    // hBrokenTime
}  // namespace

namespace {
    static const MR::ActorMoveParam hNoMoveNoTurnParam = {0.0f, 1.2f, 0.8f, 0.0f};
    static const MR::ActorMoveParam hNoMoveNoTurnOnAirParam = {0.0f, 2.0f, 0.95, 0.0f};
    static const MR::ActorMoveParam hWalkParam = {0.12f, 1.2f, 0.95f, 2.0f};
    static const MR::ActorMoveParam hSearchParam = {0.0f, 1.2f, 0.95f, 2.0f};
    static const MR::ActorMoveParam hPursueParam = {0.25f, 1.2f, 0.95f, 2.0f};
    static const MR::ActorMoveParam hHitReactionOnGroundParam = {0.0f, 1.2f, 0.8f, 5.0f};
    static const MR::ActorMoveParam hHitReactionAirParam = {0.0f, 2.0f, 0.95f, 5.0f};
};  // namespace

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
};  // namespace NrvKaron

Karon::Karon(const char* pName) : LiveActor(pName) {
    mTerritoryMover = nullptr;
    _90 = 0;
    mStopSwAnim = false;
    mScaleController = nullptr;
    mStateStarPointer = nullptr;
}

void Karon::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::useStageSwitchWriteDead(this, rIter);
    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
    }

    bool needsFixWait = false;
    if (MR::useStageSwitchReadA(this, rIter)) {
        needsFixWait = true;
    }

    if (MR::isConnectedWithRail(rIter)) {
        initRailRider(rIter);
    }

    s32 arg0 = -1;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);

    if (arg0 == -1) {
        mStopSwAnim = false;
    } else {
        mStopSwAnim = true;
    }

    initModelManagerWithAnm("Karon", nullptr, false);
    MR::connectToSceneEnemy(this);
    initBinder(70.0f, 70.0f, 0);
    MR::onCalcGravity(this);
    initSound(4, false);
    initEffectKeeper(1, nullptr, false);

    if (needsFixWait) {
        initNerve(GET_NERVE(Karon, HostTypeNrvFixWait));
    } else {
        initNerve(GET_NERVE(Karon, HostTypeNrvWait));
    }

    initHitSensor(1);
    TVec3f offset;
    f32 yScale = mScale.y;
    offset.set(0.0f, 90.0f * yScale, 0.0f);
    MR::addHitSensorEnemy(this, "body", 16, 90.0f * yScale, offset);
    MR::initShadowVolumeSphere(this, 60.0f);
    MR::initLightCtrl(this);
    MR::declareStarPiece(this, 3);
    MR::validateClipping(this);
    MR::setGroupClipping(this, rIter, 32);
    mTerritoryMover = new TerritoryMover(500.0f);
    mTerritoryMover->mCenter.set(mPosition);
    MR::initStarPointerTarget(this, 100.0f, TVec3f(0.0f, 50.0f, 0.0f));
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
    MR::startSoundSeVer(this, "SE_EM_EXPLODE_S");
}

void Karon::exeFixWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Wait");
        if (!mStopSwAnim) {
            MR::stopBck(this);
        }
    }

    mBinder->_1EC._5 = true;
    MR::moveAndTurnToPlayer(this, ::hNoMoveNoTurnParam._0, ::hNoMoveNoTurnParam._4, ::hNoMoveNoTurnParam._8, ::hNoMoveNoTurnParam._C);

    if (MR::isOnSwitchA(this)) {
        setNerve(GET_NERVE(Karon, HostTypeNrvWait));
    }
}

void Karon::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Wait");
    }

    mBinder->_1EC._5 = true;

    if (MR::isOnGround(this)) {
        MR::moveAndTurnToPlayer(this, ::hNoMoveNoTurnParam._0, ::hNoMoveNoTurnParam._4, ::hNoMoveNoTurnParam._8, ::hNoMoveNoTurnParam._C);
    } else {
        MR::moveAndTurnToPlayer(this, ::hNoMoveNoTurnOnAirParam._0, ::hNoMoveNoTurnOnAirParam._4, ::hNoMoveNoTurnOnAirParam._8,
                                ::hNoMoveNoTurnOnAirParam._C);
    }

    s32 step;
    if (isNerve(GET_NERVE(Karon, HostTypeNrvStarPieceHitWait))) {
        step = 60;
    } else {
        step = 120;
    }

    if (MR::calcDistanceToPlayer(this) < 700.0f && MR::isFaceToPlayerDegree(this, 80.0f)) {
        setNerve(GET_NERVE(Karon, HostTypeNrvSearch));
    } else if (MR::isGreaterStep(this, step)) {
        if (mRailRider != nullptr) {
            setNerve(GET_NERVE(Karon, HostTypeNrvWalkOnRail));
        } else {
            mTerritoryMover->decideNextTargetPos(this);
            setNerve(GET_NERVE(Karon, HostTypeNrvWalk));
        }
    }
}

void Karon::exeWalk() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Walk");
    }

    MR::moveAndTurnToTarget(this, mTerritoryMover->mTarget, ::hWalkParam._0, ::hWalkParam._4, ::hWalkParam._8, ::hWalkParam._C);

    if (MR::isGreaterStep(this, 180) || mTerritoryMover->isReachedTarget(this, 40.0f)) {
        setNerve(GET_NERVE(Karon, HostTypeNrvWait));
    } else if (isFallNextMove(true)) {
        setNerve(GET_NERVE(Karon, HostTypeNrvTurn));
    } else if (MR::calcDistanceToPlayer(this) < 700.0f && MR::isFaceToPlayerDegree(this, 80.0f)) {
        setNerve(GET_NERVE(Karon, HostTypeNrvSearch));
    }
}

void Karon::exeTurn() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Walk");
        mTerritoryMover->decideNextTargetPos(this);
    }

    MR::moveAndTurnToTarget(this, mTerritoryMover->mTarget, ::hSearchParam._0, ::hSearchParam._4, ::hSearchParam._8, ::hSearchParam._C);

    if (MR::calcDistanceToPlayer(this) < 700.0f && MR::isFaceToPlayerDegree(this, 80.0f)) {
        setNerve(GET_NERVE(Karon, HostTypeNrvPursue));
    } else {
        TVec3f frontVec;
        MR::calcFrontVec(&frontVec, this);
        if (MR::isGreaterStep(this, 10) && MR::isFaceToTargetHorizontalDegree(this, mTerritoryMover->mTarget, frontVec, ::hSearchParam._C * 2.0f)) {
            if (isFallNextMove(false)) {
                setNerve(GET_NERVE(Karon, HostTypeNrvTurn));
            } else {
                setNerve(GET_NERVE(Karon, HostTypeNrvWalk));
            }
        }
    }
}

void Karon::exeWalkOnRail() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Walk");
    }

    MR::moveAndTurnAlongRail(this, 600.0f, ::hWalkParam._0, ::hWalkParam._4, ::hWalkParam._8, ::hWalkParam._C, nullptr);
    MR::calcDistanceToPlayer(this);

    if (MR::calcDistanceToPlayer(this) < 700.0f && MR::isFaceToPlayerDegree(this, 80.0f)) {
        setNerve(GET_NERVE(Karon, HostTypeNrvSearch));
    }
}

void Karon::exeSearch() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Search");
        MR::startSound(this, "SE_EV_KARON_FIND");
    }

    MR::moveAndTurnToPlayer(this, ::hSearchParam._0, ::hSearchParam._4, ::hSearchParam._8, ::hSearchParam._C);

    if (MR::isActionEnd(this)) {
        setNerve(GET_NERVE(Karon, HostTypeNrvPursue));
    }
}

void Karon::exePursue() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Pursue");
        MR::invalidateClipping(this);
        mVelocity += mGravity * -10.0f;
    }

    MR::moveAndTurnToPlayer(this, ::hPursueParam._0, ::hPursueParam._4, ::hPursueParam._8, ::hPursueParam._C);

    if (1200.0f < MR::calcDistanceToPlayer(this) || !MR::isFaceToPlayerDegree(this, 80.0f)) {
        setNerve(GET_NERVE(Karon, HostTypeNrvWait));
    }
}

void Karon::endPursue() {
    MR::validateClipping(this);
}

void Karon::exeBroken() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "SpinBreak");
        MR::startSound(this, "SE_EM_KARON_BROKEN");
        MR::startSound(this, "SE_EV_KARON_DAMAGE");
        MR::offCalcGravity(this);
        MR::forceBindOnGround(this, 0.0f, 1.0f);
    }

    TVec3f velVec(0, 0, 0);
    if (MR::calcVelocityAreaOrRailMoveOnGround(&velVec, this)) {
        MR::addVelocityLimit(this, velVec * 1.0f);
        MR::moveAndTurnToPlayer(this, ::hNoMoveNoTurnParam._0, ::hNoMoveNoTurnParam._4, ::hNoMoveNoTurnParam._8, ::hNoMoveNoTurnParam._C);
        MR::calcGravity(this);
    } else if (MR::isBinded(this)) {
        const TVec3f* bindedVec = MR::getBindedNormal(this);
        mVelocity.scale(bindedVec->dot(mVelocity), *bindedVec);
    } else {
        MR::forceBindOnGround(this, 1.0f, 0.8f);
        TVec3f* gravity = &mGravity;
        mVelocity.scale(gravity->dot(mVelocity), *gravity);
    }

    mBinder->_1EC._5 = true;

    if (MR::isGreaterStep(this, 300)) {
        setNerve(GET_NERVE(Karon, HostTypeNrvRecover));
    }
}

void Karon::exeStepBroken() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "StepBreak");
        MR::startSound(this, "SE_EM_KARON_BROKEN");
        MR::startSound(this, "SE_EV_KARON_DAMAGE");
        MR::offCalcGravity(this);
        MR::forceBindOnGround(this, 0.0f, 1.0f);
    }

    TVec3f velVec(0, 0, 0);
    if (MR::calcVelocityAreaOrRailMoveOnGround(&velVec, this)) {
        MR::addVelocityLimit(this, velVec * 1.0f);
        MR::moveAndTurnToPlayer(this, ::hNoMoveNoTurnParam._0, ::hNoMoveNoTurnParam._4, ::hNoMoveNoTurnParam._8, ::hNoMoveNoTurnParam._C);
        MR::calcGravity(this);
    } else if (MR::isBinded(this)) {
        const TVec3f* bindedVec = MR::getBindedNormal(this);
        mVelocity.scale(bindedVec->dot(mVelocity), *bindedVec);
    } else {
        TVec3f* gravity = &mGravity;
        mVelocity.scale(gravity->dot(mVelocity), *gravity);
        MR::forceBindOnGround(this, 1.0f, 0.8f);
    }

    mBinder->_1EC._5 = true;

    if (MR::isGreaterStep(this, 300)) {
        setNerve(GET_NERVE(Karon, HostTypeNrvRecover));
    }
}

void Karon::exeRecover() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Recovery");
        MR::startSound(this, "SE_EM_KARON_RECOVER");
    }

    if (MR::isOnGround(this)) {
        TVec3f* gravity = &mGravity;
        mVelocity.scale(gravity->dot(mVelocity), *gravity);
    }

    MR::moveAndTurnToPlayer(this, ::hNoMoveNoTurnParam._0, ::hNoMoveNoTurnParam._4, ::hNoMoveNoTurnParam._8, ::hNoMoveNoTurnParam._C);
    mBinder->_1EC._5 = true;

    if (MR::isActionEnd(this)) {
        MR::onCalcGravity(this);
        setNerve(GET_NERVE(Karon, HostTypeNrvWait));
    }
}

void Karon::exeHitReaction() {
    if (MR::isFirstStep(this)) {
        if (isNerve(GET_NERVE(Karon, HostTypeNrvStarPieceHit))) {
            MR::startAction(this, "StarPiece");
        } else {
            MR::startAction(this, "AttackHit");
        }
    }

    MR::startLevelSound(this, "SE_EV_LV_KARON_ATK_SUCCESS");

    if (isNerve(GET_NERVE(Karon, HostTypeNrvStarPieceHit))) {
        if (MR::isOnGround(this)) {
            MR::moveAndTurnToPlayer(this, ::hNoMoveNoTurnParam._0, ::hNoMoveNoTurnParam._4, ::hNoMoveNoTurnParam._8, ::hNoMoveNoTurnParam._C);
        } else {
            MR::moveAndTurnToPlayer(this, ::hNoMoveNoTurnOnAirParam._0, ::hNoMoveNoTurnOnAirParam._4, ::hNoMoveNoTurnOnAirParam._8,
                                    ::hNoMoveNoTurnOnAirParam._C);
        }
    } else if (MR::isOnGround(this)) {
        MR::moveAndTurnToPlayer(this, ::hHitReactionOnGroundParam._0, ::hHitReactionOnGroundParam._4, ::hHitReactionOnGroundParam._8,
                                ::hHitReactionOnGroundParam._C);
    } else {
        MR::moveAndTurnToPlayer(this, ::hHitReactionAirParam._0, ::hHitReactionAirParam._4, ::hHitReactionAirParam._8, ::hHitReactionAirParam._C);
    }

    if (MR::isOnGround(this) && MR::isActionEnd(this)) {
        if (isNerve(GET_NERVE(Karon, HostTypeNrvStarPieceHit))) {
            setNerve(GET_NERVE(Karon, HostTypeNrvStarPieceHitWait));
        } else {
            setNerve(GET_NERVE(Karon, HostTypeNrvWait));
        }
    }
}

void Karon::exeDeath() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Down");
        MR::startSound(this, "SE_EM_KARON_BROKEN");
        MR::startSound(this, "SE_EV_KARON_DAMAGE");
    }

    MR::moveAndTurnToPlayer(this, ::hNoMoveNoTurnParam._0, ::hNoMoveNoTurnParam._4, ::hNoMoveNoTurnParam._8, ::hNoMoveNoTurnParam._C);

    if (MR::isActionEnd(this)) {
        kill();
    }
}

void Karon::exeSinkDown() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "SinkDown");
        MR::invalidateShadow(this, nullptr);
    }

    if (MR::isOnGround(this)) {
        TVec3f* gravity = &mGravity;
        mVelocity.scale(gravity->dot(mVelocity), *gravity);
    }

    MR::startLevelSound(this, "SE_EV_LV_KARON_SINK");
    MR::startLevelSound(this, "SE_EM_LV_KARON_SINK");
    MR::moveAndTurnToPlayer(this, ::hNoMoveNoTurnParam._0, ::hNoMoveNoTurnParam._4, ::hNoMoveNoTurnParam._8, ::hNoMoveNoTurnParam._C);

    if (MR::isActionEnd(this)) {
        kill();
    }
}

void Karon::exeBindStarPointer() {
    const Nerve* nerve = GET_NERVE(Karon, HostTypeNrvWait);
    mBinder->_1EC._5 = true;
    if (MR::updateActorStateAndNextNerve(this, mStateStarPointer, nerve)) {
        return;
    }
}

void Karon::endBindStarPointer() {
    mStateStarPointer->kill();
}

void Karon::control() {
    mScaleController->updateNerve();

    if (MR::isInDeath(this, TVec3f(0.0f, 0.0f, 0.0f)) || MR::isInWater(mPosition) || MR::isInDarkMatter(mPosition)) {
        kill();
        return;
    }

    if (!isNerve(GET_NERVE(Karon, HostTypeNrvStepBroken)) && !isNerve(GET_NERVE(Karon, HostTypeNrvBroken))) {
        TVec3f velVec(0, 0, 0);
        if (MR::calcVelocityAreaOrRailMoveOnGround(&velVec, this)) {
            MR::addVelocityLimit(this, velVec * 1.0f);
        }
    }

    if ((isNerve(GET_NERVE(Karon, HostTypeNrvWait)) || isNerve(GET_NERVE(Karon, HostTypeNrvStarPieceHitWait)) ||
         isNerve(GET_NERVE(Karon, HostTypeNrvWalk)) || isNerve(GET_NERVE(Karon, HostTypeNrvWalkOnRail)) ||
         isNerve(GET_NERVE(Karon, HostTypeNrvSearch)) || isNerve(GET_NERVE(Karon, HostTypeNrvPursue))) &&
        mStateStarPointer->tryStartPointBind()) {
        setNerve(GET_NERVE(Karon, HostTypeNrvBindStarPointer));
        return;
    }

    if (!isNerve(GET_NERVE(Karon, HostTypeNrvBroken)) && !isNerve(GET_NERVE(Karon, HostTypeNrvStepBroken)) &&
        !isNerve(GET_NERVE(Karon, HostTypeNrvRecover)) && !isNerve(GET_NERVE(Karon, HostTypeNrvDeath)) &&
        !isNerve(GET_NERVE(Karon, HostTypeNrvSinkDown)) && MR::isBindedGroundSinkDeath(this)) {
        setNerve(GET_NERVE(Karon, HostTypeNrvSinkDown));
    }
}

bool Karon::isFallNextMove(bool a1) {
    if (a1) {
        return MR::isFallOrDangerNextMove(this, 150.0f, 140.0f, 150.0f);
    }

    TVec3f frontVec;
    MR::calcFrontVec(&frontVec, this);
    return MR::isFallOrDangerNextMove(mPosition, frontVec, mGravity, 150.0f, 140.0f, 150.0f);
}

void Karon::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(Karon, HostTypeNrvBroken)) || isNerve(GET_NERVE(Karon, HostTypeNrvStepBroken)) ||
        isNerve(GET_NERVE(Karon, HostTypeNrvRecover)) || isNerve(GET_NERVE(Karon, HostTypeNrvDeath))) {
        return;
    }

    if (getSensor("body") == pSender) {
        if (MR::isSensorPlayer(pReceiver)) {
            if (MR::isPlayerExistUp(this, getSensor("body")->mRadius, 0.25f) && MR::isOnGroundPlayer()) {
                if (MR::sendMsgTouchJump(pReceiver, pSender)) {
                    setNerve(GET_NERVE(Karon, HostTypeNrvStepBroken));
                }
            } else if (!isNerve(GET_NERVE(Karon, HostTypeNrvHitReaction)) && !isNerve(GET_NERVE(Karon, HostTypeNrvFixWait)) &&
                       !MR::isPlayerExistUp(this, getSensor("body")->mRadius, 0.25f) && MR::isOnGroundPlayer() && !MR::isPlayerHipDropFalling() &&
                       !MR::isPlayerHipDropLand() && MR::sendMsgEnemyAttack(pReceiver, pSender)) {
                TVec3f v18(pSender->mPosition);
                v18 -= pReceiver->mPosition;
                MR::vecKillElement(v18, mGravity, &v18);
                MR::normalizeOrZero(&v18);
                mVelocity.add(v18 * 10.0f);
                mVelocity.add(mGravity * -20.0f);
                setNerve(GET_NERVE(Karon, HostTypeNrvHitReaction));
            } else {
                MR::sendMsgPushAndKillVelocityToTarget(this, pReceiver, pSender);
            }
        } else if (MR::isSensorEnemy(pReceiver)) {
            MR::sendMsgPushAndKillVelocityToTarget(this, pReceiver, pSender);
        }
    }
}

bool Karon::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(Karon, HostTypeNrvBroken)) || isNerve(GET_NERVE(Karon, HostTypeNrvStepBroken)) ||
        isNerve(GET_NERVE(Karon, HostTypeNrvDeath)) || isNerve(GET_NERVE(Karon, HostTypeNrvSinkDown))) {
        return false;
    }

    if (MR::isMsgPlayerTrample(msg)) {
        if (MR::isPlayerElementModeInvincible()) {
            MR::appearStarPiece(this, mPosition, 3, 10.0f, 40.0f, false);
            MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
            setNerve(GET_NERVE(Karon, HostTypeNrvDeath));
        } else {
            setNerve(GET_NERVE(Karon, HostTypeNrvStepBroken));
        }
        return true;
    }

    if (MR::isMsgPlayerHipDrop(msg)) {
        setNerve(GET_NERVE(Karon, HostTypeNrvStepBroken));
        return true;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        setNerve(GET_NERVE(Karon, HostTypeNrvStarPieceHit));
        return true;
    }

    if (MR::isMsgPlayerSpinAttack(msg) || MR::isMsgJetTurtleAttack(msg)) {
        MR::startBlowHitSound(this);
        setNerve(GET_NERVE(Karon, HostTypeNrvBroken));
        return true;
    }

    if (MR::isMsgFireBallAttack(msg)) {
        mScaleController->startHitReaction();
        setNerve(GET_NERVE(Karon, HostTypeNrvStarPieceHit));
        return true;
    }

    if (MR::isMsgInvincibleAttack(msg)) {
        setNerve(GET_NERVE(Karon, HostTypeNrvDeath));
        MR::appearStarPiece(this, mPosition, 3, 10.0f, 40.0f, false);
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
        MR::startBlowHitSound(this);
        return true;
    }

    return false;
}

bool Karon::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgExplosionAttack(msg)) {
        kill();
        MR::appearStarPiece(this, mPosition, 3, 10.0f, 40.0f, false);
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
        return true;
    }

    return false;
}

bool Karon::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pSender)) {
        return false;
    }

    if (isNerve(GET_NERVE(Karon, HostTypeNrvBroken)) || isNerve(GET_NERVE(Karon, HostTypeNrvStepBroken)) ||
        isNerve(GET_NERVE(Karon, HostTypeNrvRecover)) || isNerve(GET_NERVE(Karon, HostTypeNrvDeath))) {
        return false;
    }

    MR::addVelocityFromPush(this, 2.0f, pSender, pReceiver);
    return true;
}

bool Karon::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgInhaleBlackHole(msg)) {
        kill();
        return true;
    }

    if (MR::isMsgStartPowerStarGet(msg)) {
        kill();
        return true;
    }

    return false;
}

void Karon::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
    MR::setBaseScale(this, mScaleController->_C * mScale);
}

Karon::~Karon() {
}
