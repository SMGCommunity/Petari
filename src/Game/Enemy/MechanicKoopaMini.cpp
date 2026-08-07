#include "Game/Enemy/MechanicKoopaMini.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

void MechanicKoopaMini_FORCE_MATCH_SDATA2() {
    (void)0.0f;
    (void)3.0f;
    (void)80.0f;
    (void)130.0f;
}

namespace NrvMechanicKoopaMini {
    NEW_NERVE(HostTypeNrvWait, MechanicKoopaMini, Wait);
    NEW_NERVE(HostTypeNrvWalkOnRail, MechanicKoopaMini, WalkOnRail);
    NEW_NERVE(HostTypeNrvTurn, MechanicKoopaMini, Turn);
    NEW_NERVE(HostTypeNrvJumpStart, MechanicKoopaMini, JumpStart);
    NEW_NERVE(HostTypeNrvJumpEnd, MechanicKoopaMini, JumpEnd);
    NEW_NERVE(HostTypeNrvFind, MechanicKoopaMini, Find);
    NEW_NERVE(HostTypeNrvPursue, MechanicKoopaMini, Pursue);
    NEW_NERVE(HostTypeNrvPreFireAttack, MechanicKoopaMini, PreFireAttack);
    NEW_NERVE_ONEND(HostTypeNrvFireAttack, MechanicKoopaMini, FireAttack, FireAttack);
    NEW_NERVE(HostTypeNrvFireAttackEnd, MechanicKoopaMini, FireAttackEnd);
    NEW_NERVE(HostTypeNrvAttackHit, MechanicKoopaMini, AttackHit);
    NEW_NERVE(HostTypeNrvSpinHit, MechanicKoopaMini, SpinHit);
    NEW_NERVE(HostTypeNrvTrample, MechanicKoopaMini, Trample);
    NEW_NERVE(HostTypeNrvHipDropped, MechanicKoopaMini, HipDropped);
    NEW_NERVE_ONEND(HostTypeNrvBindStarPointer, MechanicKoopaMini, BindStarPointer, BindStarPointer);
};  // namespace NrvMechanicKoopaMini

namespace {
    static const MR::ActorMoveParam hNoMoveNoTurnParam = {0.0f, 2.0f, 0.8f, 0.0f};
    static const MR::ActorMoveParam hOnAirParam = {0.0f, 2.0f, 0.98f, 0.0f};
    static const MR::ActorMoveParam hOnAirTurnParam = {0.0f, 2.0f, 0.98f, 2.0f};
    static const MR::ActorMoveParam hOnAirMoveParam = {0.2f, 2.0f, 0.98f, 2.0f};
    static const MR::ActorMoveParam hWalkParam = {0.4f, 2.0f, 0.8f, 2.0f};
    static const MR::ActorMoveParam hPursueParam = {1.3f, 2.0f, 0.8f, 2.0f};
    static const MR::ActorMoveParam hPreFireAttackParam = {0.0f, 2.0f, 0.8f, 0.0f};
    static const MR::ActorMoveParam hFireAttackParam = {0.0f, 2.0f, 0.8f, 1.0f};
    static const MR::ActorMoveParam hHitReactionOnGroundParam = {0.0f, 1.2f, 0.8f, 5.0f};
    static const MR::ActorMoveParam hHitReactionAirParam = {0.0f, 2.0f, 0.95f, 5.0f};
    static const MR::ActorMoveParam hJumpOnGroundParam = {0.0f, 1.2f, 0.8f, 0.0f};
    static const MR::ActorMoveParam hJumpAirParam = {0.5f, 2.0f, 0.95f, 0.0f};
    static const MR::ActorMoveParam hTurnParam = {0.0f, 2.0f, 0.8f, 2.0f};
    static const TVec3f hBinderOffset(0.0f, 110.0f, 20.0f);
    static const f32 hTrampleUpRangeSub = 80.0f;
    static const f32 hPushUpRangeSub = 130.0f;
};  // namespace

MechanicKoopaMini::MechanicKoopaMini(const char* pName) : LiveActor(pName) {
    mIsAppearJumping = false;
    mIsForbidPause = false;
    mScaleController = nullptr;
    mBindStarPointer = nullptr;
}

MechanicKoopaMini::~MechanicKoopaMini() {
}

void MechanicKoopaMini::onAppearSwitchOn() {
    if (mIsAppearJumping) {
        setNerve(&NrvMechanicKoopaMini::HostTypeNrvJumpStart::sInstance);
    } else {
        setNerve(&NrvMechanicKoopaMini::HostTypeNrvWait::sInstance);
    }
}

void MechanicKoopaMini::init(const JMapInfoIter& rIter) {
    if (MR::isValidInfo(rIter)) {
        MR::isConnectedWithRail(rIter);
    }

    MR::initDefaultPos(this, rIter);
    MR::useStageSwitchWriteDead(this, rIter);
    MR::useStageSwitchSleep(this, rIter);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::listenStageSwitchOnAppear(this, MR::Functor_Inline(this, &::MechanicKoopaMini::onAppearSwitchOn));
    }

    s32 arg = -1;
    MR::getJMapInfoArg0NoInit(rIter, &arg);
    if (arg == -1) {
        mIsAppearJumping = false;
    } else {
        mIsAppearJumping = true;
    }

    arg = -1;
    MR::getJMapInfoArg1NoInit(rIter, &arg);
    if (arg == -1) {
        mIsForbidPause = false;
    } else {
        mIsForbidPause = true;
    }

    initRailRider(rIter);
    initModelManagerWithAnm("MechanicKoopaMini", nullptr, false);
    MR::connectToSceneEnemy(this);
    initBinder(120.0f, 0.0f, 0);
    MR::setBinderOffsetVec(this, &::hBinderOffset, true);
    MR::onCalcGravity(this);
    initSound(3, false);
    MR::initLightCtrl(this);
    initEffectKeeper(1, nullptr, false);
    initNerve(&NrvMechanicKoopaMini::HostTypeNrvWalkOnRail::sInstance);
    initHitSensor(3);
    f32 yScale = mScale.y;
    MR::addHitSensorAtJointEnemy(this, "body", "JointRoot", 16, 130.0f * yScale, TVec3f(48.0f, 13.0f, 0.0f));
    MR::addHitSensorCallbackEnemyAttack(this, "fire", 16, 29.0f * yScale);
    MR::initShadowFromCSV(this, "Shadow");
    MR::declareCoin(this, 1);
    MR::initStarPointerTarget(this, 70.0f, TVec3f(0.0f, 35.0f, 0.0f));
    MR::validateClipping(this);
    MR::tryRegisterDemoCast(this, rIter);
    mScaleController = new AnimScaleController(nullptr);
    mScaleController->setParamTight();
    mBindStarPointer = new WalkerStateBindStarPointer(this, mScaleController);

    if (MR::isValidSwitchAppear(this)) {
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

void MechanicKoopaMini::initAfterPlacement() {
    MR::trySetMoveLimitCollision(this);
}

void MechanicKoopaMini::makeActorAppeared() {
    LiveActor::makeActorAppeared();
    getSensor("fire")->invalidate();
}

void MechanicKoopaMini::kill() {
    LiveActor::kill();
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }

    MR::emitEffect(this, "Death");
    MR::startSound(this, "SE_EM_EXPLODE_S");
}

void MechanicKoopaMini::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Wait");
    }

    MR::moveAndTurnToPlayer(this, ::hNoMoveNoTurnParam._0, ::hNoMoveNoTurnParam._4, ::hNoMoveNoTurnParam._8, ::hNoMoveNoTurnParam._C);

    if (MR::calcDistanceToPlayer(this) < 900.0f && MR::isFaceToPlayerDegree(this, 120.0f)) {
        setNerve(&NrvMechanicKoopaMini::HostTypeNrvFind::sInstance);
    } else if (MR::isGreaterStep(this, 120)) {
        setNerve(&NrvMechanicKoopaMini::HostTypeNrvWalkOnRail::sInstance);
    }
}

void MechanicKoopaMini::exeWalkOnRail() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Walk");
    }

    bool canTurn = false;
    if (MR::isOnGround(this)) {
        MR::moveAndTurnAlongRail(this, 600.0f, ::hWalkParam._0, ::hWalkParam._4, ::hWalkParam._8, ::hWalkParam._C, &canTurn);
        TVec3f frontVec;
        MR::calcFrontVec(&frontVec, this);
        if (MR::isBindedWallOfMap(this)) {
            TVec3f vecc(*MR::getWallNormal(this));
            if (frontVec.dot(vecc) < -0.99f) {
                setNerve(&NrvMechanicKoopaMini::HostTypeNrvJumpStart::sInstance);
                return;
            }
        }

        if (MR::calcDistanceToPlayer(this) < 900.0f && MR::isFaceToPlayerDegree(this, 120.0f)) {
            setNerve(&NrvMechanicKoopaMini::HostTypeNrvFind::sInstance);
            return;
        }
    } else {
        MR::moveAndTurnAlongRail(this, 600.0f, ::hOnAirMoveParam._0, ::hOnAirMoveParam._4, ::hOnAirMoveParam._8, ::hOnAirMoveParam._C, &canTurn);
    }

    if (canTurn) {
        setNerve(&NrvMechanicKoopaMini::HostTypeNrvTurn::sInstance);
    } else if (!mIsForbidPause && MR::isGreaterStep(this, 120)) {
        setNerve(&NrvMechanicKoopaMini::HostTypeNrvWait::sInstance);
    }
}

void MechanicKoopaMini::exeTurn() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Turn");
    }

    TVec3f railDir(MR::getRailDirection(this));
    if (MR::isOnGround(this)) {
        MR::moveAndTurnToDirection(this, railDir, ::hTurnParam._0, ::hTurnParam._4, ::hTurnParam._8, ::hTurnParam._C);
    } else {
        MR::moveAndTurnToDirection(this, railDir, ::hOnAirParam._0, ::hOnAirParam._4, ::hOnAirParam._8, ::hOnAirParam._C);
    }

    TVec3f vec(mPosition);
    vec.add(railDir);
    if (MR::isFaceToTargetDegree(this, vec, 5.0f)) {
        setNerve(&NrvMechanicKoopaMini::HostTypeNrvWalkOnRail::sInstance);
    }
}

void MechanicKoopaMini::exeJumpStart() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "JumpStart");
    }

    if (MR::isOnGround(this)) {
        MR::moveAndTurnToPlayer(this, ::hJumpOnGroundParam._0, ::hJumpOnGroundParam._4, ::hJumpOnGroundParam._8, ::hJumpOnGroundParam._C);
    } else {
        MR::moveAndTurnToPlayer(this, ::hJumpAirParam._0, ::hJumpAirParam._4, ::hJumpAirParam._8, ::hJumpAirParam._C);
    }

    if (MR::isStep(this, (s32)MR::getBckFrameMax(this) - 3)) {
        MR::addVelocityJump(this, 45.0f);
        MR::startSound(this, "SE_EM_MKOOPAMINI_JUMP");
    }

    if (MR::isActionEnd(this)) {
        setNerve(&NrvMechanicKoopaMini::HostTypeNrvJumpEnd::sInstance);
    }
}

void MechanicKoopaMini::exeJumpEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "JumpEnd");
    }

    if (MR::isOnGround(this)) {
        MR::moveAndTurnToPlayer(this, ::hJumpOnGroundParam._0, ::hJumpOnGroundParam._4, ::hJumpOnGroundParam._8, ::hJumpOnGroundParam._C);
    } else {
        MR::moveAndTurnToPlayer(this, ::hJumpAirParam._0, ::hJumpAirParam._4, ::hJumpAirParam._8, ::hJumpAirParam._C);
    }

    if (MR::isActionEnd(this)) {
        setNerve(&NrvMechanicKoopaMini::HostTypeNrvWalkOnRail::sInstance);
    }
}

void MechanicKoopaMini::exeFind() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Find");
        MR::addVelocityJump(this, 26.0f);
        MR::startSound(this, "SE_EM_MKOOPAMINI_JUMP");
    }

    if (MR::isOnGround(this)) {
        MR::moveAndTurnToPlayer(this, ::hTurnParam._0, ::hTurnParam._4, ::hTurnParam._8, ::hTurnParam._C);
    } else {
        MR::moveAndTurnToPlayer(this, ::hOnAirTurnParam._0, ::hOnAirTurnParam._4, ::hOnAirTurnParam._8, ::hOnAirTurnParam._C);
    }

    if (MR::isActionEnd(this)) {
        setNerve(&NrvMechanicKoopaMini::HostTypeNrvPursue::sInstance);
    }
}

void MechanicKoopaMini::exePursue() {
    TVec3f frontVec;
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Pursue");
        MR::invalidateClipping(this);
        mVelocity.set(mGravity * -10.0f);
    }

    if (MR::isOnGround(this)) {
        MR::moveAndTurnToPlayer(this, ::hPursueParam._0, ::hPursueParam._4, ::hPursueParam._8, ::hPursueParam._C);
        MR::calcFrontVec(&frontVec, this);
        if (MR::isBindedWall(this)) {
            TVec3f vecc(*MR::getWallNormal(this));
            if (frontVec.dot(vecc) < -0.99f) {
                setNerve(&NrvMechanicKoopaMini::HostTypeNrvJumpStart::sInstance);
            }
        }
    } else {
        MR::moveAndTurnToPlayer(this, ::hOnAirMoveParam._0, ::hOnAirMoveParam._4, ::hOnAirMoveParam._8, ::hOnAirMoveParam._C);
        MR::calcFrontVec(&frontVec, this);
    }

    f32 distance = MR::calcDistanceToPlayer(this);
    if (1100.0f < distance) {
        setNerve(&NrvMechanicKoopaMini::HostTypeNrvWalkOnRail::sInstance);
    } else if (distance < 300.0f && MR::isFaceToPlayerHorizontalDegree(this, frontVec, 5.729578f) && MR::isOnGround(this)) {
        setNerve(&NrvMechanicKoopaMini::HostTypeNrvPreFireAttack::sInstance);
    }
}

void MechanicKoopaMini::exePreFireAttack() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "PreFire");
        MR::invalidateClipping(this);
        mVelocity.set(mGravity * -10.0f);
    }

    if (MR::isLessEqualStep(this, 60)) {
        MR::startLevelSound(this, "SE_EM_LV_MKOOPAMINI_PRE_FIRE");
    }

    if (MR::isOnGround(this)) {
        MR::moveAndTurnToPlayer(this, ::hPreFireAttackParam._0, ::hPreFireAttackParam._4, ::hPreFireAttackParam._8, ::hPreFireAttackParam._C);
    } else {
        MR::moveAndTurnToPlayer(this, ::hOnAirParam._0, ::hOnAirParam._4, ::hOnAirParam._8, ::hOnAirParam._C);
    }

    if (MR::isActionEnd(this)) {
        setNerve(&NrvMechanicKoopaMini::HostTypeNrvFireAttack::sInstance);
    }
}

void MechanicKoopaMini::exeFireAttack() {
    if (MR::isFirstStep(this)) {
        getSensor("fire")->validate();
        MR::emitEffect(this, "Fire");
        MR::startAction(this, "Fire");
    }

    if (MR::isOnGround(this)) {
        MR::moveAndTurnToPlayer(this, ::hFireAttackParam._0, ::hFireAttackParam._4, ::hFireAttackParam._8, ::hFireAttackParam._C);
    } else {
        MR::moveAndTurnToPlayer(this, ::hOnAirParam._0, ::hOnAirParam._4, ::hOnAirParam._8, ::hOnAirParam._C);
    }

    if (MR::isGreaterStep(this, 120)) {
        setNerve(&NrvMechanicKoopaMini::HostTypeNrvFireAttackEnd::sInstance);
    } else if (MR::isLessEqualStep(this, 90)) {
        MR::startLevelSound(this, "SE_EM_LV_MKOOPAMINI_FIRE");
    }
}

void MechanicKoopaMini::endFireAttack() {
    MR::deleteEffect(this, "Fire");
    getSensor("fire")->invalidate();
}

void MechanicKoopaMini::exeFireAttackEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "FireEnd");
    }

    if (MR::isOnGround(this)) {
        MR::moveAndTurnToPlayer(this, ::hNoMoveNoTurnParam._0, ::hNoMoveNoTurnParam._4, ::hNoMoveNoTurnParam._8, ::hNoMoveNoTurnParam._C);
    } else {
        MR::moveAndTurnToPlayer(this, ::hOnAirParam._0, ::hOnAirParam._4, ::hOnAirParam._8, ::hOnAirParam._C);
    }

    if (MR::isActionEnd(this)) {
        setNerve(&NrvMechanicKoopaMini::HostTypeNrvWalkOnRail::sInstance);
    }
}

void MechanicKoopaMini::exeAttackHit() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "AttackHit");
    }

    if (MR::isOnGround(this)) {
        MR::moveAndTurnToPlayer(this, ::hHitReactionOnGroundParam._0, ::hHitReactionOnGroundParam._4, ::hHitReactionOnGroundParam._8,
                                ::hHitReactionOnGroundParam._C);
    } else {
        MR::moveAndTurnToPlayer(this, ::hHitReactionAirParam._0, ::hHitReactionAirParam._4, ::hHitReactionAirParam._8, ::hHitReactionAirParam._C);
    }

    if (MR::isOnGround(this) && MR::isActionEnd(this)) {
        setNerve(&NrvMechanicKoopaMini::HostTypeNrvWait::sInstance);
    }
}

void MechanicKoopaMini::exeSpinHit() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "SpinHit");
        MR::startBlowHitSound(this);
        MR::startSound(this, "SE_EM_MKOOPAMINI_SPIN_HIT");
    }

    if (MR::isGreaterEqualStep(this, 60)) {
        MR::startLevelSound(this, "SE_EM_LV_MKOOPAMINI_PRE_FIRE");
    }

    if (MR::isOnGround(this)) {
        MR::moveAndTurnToPlayer(this, ::hNoMoveNoTurnParam._0, ::hNoMoveNoTurnParam._4, ::hNoMoveNoTurnParam._8, ::hNoMoveNoTurnParam._C);
    } else {
        MR::moveAndTurnToPlayer(this, ::hOnAirParam._0, ::hOnAirParam._4, ::hOnAirParam._8, ::hOnAirParam._C);
    }

    if (MR::isActionEnd(this)) {
        setNerve(&NrvMechanicKoopaMini::HostTypeNrvFireAttack::sInstance);
    }
}

void MechanicKoopaMini::exeTrample() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Trample");
    }

    if (MR::isGreaterEqualStep(this, 60)) {
        MR::startLevelSound(this, "SE_EM_LV_MKOOPAMINI_PRE_FIRE");
    }

    if (MR::isOnGround(this)) {
        MR::moveAndTurnToPlayer(this, ::hNoMoveNoTurnParam._0, ::hNoMoveNoTurnParam._4, ::hNoMoveNoTurnParam._8, ::hNoMoveNoTurnParam._C);
    } else {
        MR::moveAndTurnToPlayer(this, ::hOnAirParam._0, ::hOnAirParam._4, ::hOnAirParam._8, ::hOnAirParam._C);
    }

    if (MR::isActionEnd(this)) {
        setNerve(&NrvMechanicKoopaMini::HostTypeNrvFireAttack::sInstance);
    }
}

void MechanicKoopaMini::exeHipDropped() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Break");
        MR::invalidateHitSensors(this);
        MR::startSound(this, "SE_EM_MKOOPAMINI_BREAK");
    }

    if (MR::isActionEnd(this)) {
        kill();
        MR::appearCoinPop(this, mPosition, 1);
    }
}

void MechanicKoopaMini::exeBindStarPointer() {
    MR::updateActorStateAndNextNerve(this, mBindStarPointer, &NrvMechanicKoopaMini::HostTypeNrvWalkOnRail::sInstance);
}

void MechanicKoopaMini::endBindStarPointer() {
    mBindStarPointer->kill();
}

void MechanicKoopaMini::control() {
    mScaleController->updateNerve();
    if (MR::isInDeath(this, TVec3f(0.0f, 0.0f, 0.0f)) || MR::isInWater(mPosition)) {
        kill();
    } else if ((isNerve(&NrvMechanicKoopaMini::HostTypeNrvFireAttack::sInstance) ||
                isNerve(&NrvMechanicKoopaMini::HostTypeNrvPreFireAttack::sInstance) ||
                isNerve(&NrvMechanicKoopaMini::HostTypeNrvWalkOnRail::sInstance) || isNerve(&NrvMechanicKoopaMini::HostTypeNrvPursue::sInstance)) &&
               mBindStarPointer->tryStartPointBind()) {
        setNerve(&NrvMechanicKoopaMini::HostTypeNrvBindStarPointer::sInstance);
    }
}

void MechanicKoopaMini::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!isNerve(&NrvMechanicKoopaMini::HostTypeNrvHipDropped::sInstance)) {
        if (getSensor("fire") == pSender) {
            if (MR::isSensorPlayer(pReceiver)) {
                MR::sendMsgEnemyAttackFire(pReceiver, pSender);
            }
        } else if (MR::isSensorPlayer(pReceiver)) {
            if (!MR::isPlayerHipDropFalling()) {
                TVec3f pos(pReceiver->mPosition);
                pos -= pSender->mPosition;
                f32 SenderRad = pSender->mRadius;
                f32 ReceiverRad = pReceiver->mRadius;
                if (!(((ReceiverRad + SenderRad) - ::hPushUpRangeSub) < pos.dot(-mGravity))) {
                    MR::sendMsgPush(pReceiver, pSender);
                }
            }
        } else if (MR::isSensorEnemy(pReceiver)) {
            MR::sendMsgPush(pReceiver, pSender);
        }
    }
}

bool MechanicKoopaMini::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (getSensor("fire") == pReceiver) {
        return false;
    }

    if (isNerve(&NrvMechanicKoopaMini::HostTypeNrvHipDropped::sInstance)) {
        return false;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        setNerve(&NrvMechanicKoopaMini::HostTypeNrvSpinHit::sInstance);
        return true;
    }

    if (MR::isMsgPlayerTrample(msg)) {
        TVec3f v16(pSender->mPosition);
        v16 -= pReceiver->mPosition;
        f32 ReceiverRad = pReceiver->mRadius;
        f32 SenderRad = pSender->mRadius;
        if (((SenderRad + ReceiverRad) - ::hTrampleUpRangeSub) < v16.dot(-mGravity)) {
            return false;
        } else {
            MR::startSound(this, "SE_EM_MKOOPAMINI_TRAMPLE");
            if (isNerve(&NrvMechanicKoopaMini::HostTypeNrvSpinHit::sInstance)) {
                return true;
            }

            if (!isNerve(&NrvMechanicKoopaMini::HostTypeNrvTrample::sInstance)) {
                setNerve(&NrvMechanicKoopaMini::HostTypeNrvTrample::sInstance);
            }
            return true;
        }
    }

    if (MR::isMsgPlayerHipDrop(msg) && !isNerve(&NrvMechanicKoopaMini::HostTypeNrvHipDropped::sInstance)) {
        setNerve(&NrvMechanicKoopaMini::HostTypeNrvHipDropped::sInstance);
        return true;
    }

    if (MR::isMsgPlayerSpinAttack(msg)) {
        if (!isNerve(&NrvMechanicKoopaMini::HostTypeNrvSpinHit::sInstance) || !MR::isLessStep(this, 120)) {
            setNerve(&NrvMechanicKoopaMini::HostTypeNrvSpinHit::sInstance);
        }
        return true;
    }

    return false;
}

bool MechanicKoopaMini::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgExplosionAttack(msg)) {
        kill();
        MR::appearCoinPop(this, mPosition, 1);
        return true;
    }

    return false;
}

bool MechanicKoopaMini::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgInhaleBlackHole(msg)) {
        kill();
        return true;
    }

    return false;
}

bool MechanicKoopaMini::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorPlayer(pSender)) {
        MR::addVelocityFromPush(this, 3.0f, pSender, pReceiver);
        return true;
    }

    return false;
}

void MechanicKoopaMini::updateHitSensor(HitSensor* pSensor) {
    TVec3f up, front;
    MR::calcUpVec(&up, this);
    MR::calcFrontVec(&front, this);
    f32 dot = front.dot(*MR::getPlayerCenterPos() - mPosition);
    f32 nerveRate = 400.0f * MR::calcNerveRate(this, 10, 30);
    f32 frontDist = MR::clamp(dot, 0.0f, nerveRate);

    pSensor->mPosition.set(mPosition);
    pSensor->mPosition.add(up * getSensor("body")->getRadius());
    pSensor->mPosition.add(front * frontDist);
}

void MechanicKoopaMini::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
    MR::setBaseScale(this, mScaleController->_C * mScale);
}
