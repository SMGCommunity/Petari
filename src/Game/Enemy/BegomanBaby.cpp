#include "Game/Enemy/BegomanBaby.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JointController.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <JSystem/JMath/JMATrigonometric.hpp>

namespace NrvBegomanBaby {
    NEW_NERVE_ONEND(HostTypeNrvNoCalcWait, BegomanBaby, NoCalcWait, NoCalcWait);
    NEW_NERVE(HostTypeNrvWait, BegomanBaby, Wait);
    NEW_NERVE(HostTypeNrvSignAttack, BegomanBaby, SignAttack);
    NEW_NERVE(HostTypeNrvPursue, BegomanBaby, Pursue);
    NEW_NERVE(HostTypeNrvTurn, BegomanBaby, Turn);
    NEW_NERVE(HostTypeNrvBrake, BegomanBaby, Brake);
    NEW_NERVE(HostTypeNrvStepBack, BegomanBaby, StepBack);
    NEW_NERVE(HostTypeNrvReturn, BegomanBaby, Return);
    NEW_NERVE(HostTypeNrvProvoke, BegomanBaby, Provoke);
    NEW_NERVE(HostTypeNrvTrample, BegomanBaby, Trample);
    NEW_NERVE(HostTypeNrvHitReaction, BegomanBaby, HitReaction);
    NEW_NERVE(HostTypeNrvTired, BegomanBaby, Tired);
    NEW_NERVE(HostTypeNrvBlow, BegomanBaby, Blow);
    NEW_NERVE(HostTypeNrvAfterLaunch, BegomanBaby, AfterLaunch);
    NEW_NERVE(HostTypeNrvAfterLaunchOnGround, BegomanBaby, AfterLaunchOnGround);
    NEW_NERVE(HostTypeNrvLaunchFromGuarder, BegomanBaby, LaunchFromGuarder);
    NEW_NERVE(HostTypeNrvLaunchTurn, BegomanBaby, LaunchTurn);
    NEW_NERVE(HostTypeNrvInWater, BegomanBaby, InWater);
    NEW_NERVE(HostTypeNrvKeepDistance, BegomanBaby, KeepDistance);
    NEW_NERVE_ONEND(HostTypeNrvBindStarPointer, BegomanBaby, BindStarPointer, BindStarPointer);
};  // namespace NrvBegomanBaby

namespace {
    const MR::ActorMoveParam hWaitParam = {0.0f, 3.0f, 0.95f, 1.0f};
    const MR::ActorMoveParam hSignAttackParam = {0.0f, 3.0f, 0.8f, 3.0f};
    const MR::ActorMoveParam hPursueParam = {0.4f, 3.0f, 0.97f, 3.0f};
    const MR::ActorMoveParam hTurnParam = {0.4f, 3.0f, 0.95f, 0.0f};
    const MR::ActorMoveParam hStopParam = {0.0f, 3.0f, 0.95f, 0.0f};
    const MR::ActorMoveParam hHitReactionParam = {0.0f, 3.0f, 0.95f, 0.0f};
    const MR::ActorMoveParam hTiredParam = {0.2f, 1.0f, 0.95f, 3.0f};

};  // namespace

BegomanBaby::BegomanBaby(const char* pName) : BegomanBase(pName), mBabyDelegator(), mHost(), mAppearThreeStarPiece() {
    mWaterColumnMatrix.identity();
}

BegomanBaby::BegomanBaby(NameObj* pHost, const char* pName) : BegomanBase(pName), mBabyDelegator(), mHost(pHost), mAppearThreeStarPiece() {
    mWaterColumnMatrix.identity();
}

void BegomanBaby::init(const JMapInfoIter& rIter) {
    initCore(rIter, "BegomanBaby", false);
    MR::declareStarPiece(this, 3);

    if (mHost == nullptr) {
        mHost = this;
    }
    initBinder(45.0f, 45.0f, 0);
    initNerve(GET_NERVE(BegomanBaby, HostTypeNrvWait));
    initSensor(0, 40.0f, 80.0f, "Locator1");

    mBabyDelegator = MR::createJointDelegatorWithNullChildFunc(this, &BegomanBaby::calcHeadJoint, "BabyBegomanFace");

    initEffectAndStarPointerBind();
    MR::setEffectHostMtx(this, "WaterColumn", mWaterColumnMatrix);
    initSound(8, false);
    initShadow(35.0f, "Locator1");

    MR::tryRegisterDemoCast(this, rIter);
    if (MR::isDemoCast(this, nullptr) && MR::isRegisteredDemoActionAppear(this)) {
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

void BegomanBaby::appear() {
    BegomanBase::appear();
    setNerve(GET_NERVE(BegomanBaby, HostTypeNrvWait));
    MR::validateExCollisionParts(this);
}

void BegomanBaby::appearFromLaunch(const TVec3f& rVec1, const TVec3f& rVec2) {
    BegomanBase::appear();
    MR::emitEffect(this, "Death");
    MR::startSound(this, "SE_EM_BEGOMAN_APPEAR");
    setNerve(GET_NERVE(BegomanBaby, HostTypeNrvAfterLaunch));

    TVec3f vec(mPosition);

    vec -= rVec1;
    MR::turnVecToPlane(&vec, vec, rVec2);
    MR::normalizeOrZero(&vec);

    mTargetVec.set(vec);
    mFaceVec.set(vec);
}

void BegomanBaby::appearFromGuarder() {
    BegomanBase::appear();
    setNerve(GET_NERVE(BegomanBaby, HostTypeNrvLaunchFromGuarder));
}

void BegomanBaby::kill() {
    BegomanBase::kill();

    MR::emitEffect(this, "Death");
    MR::startSound(this, "SE_EM_EXPLODE_S");
}

void BegomanBaby::killWithGenItem() {
    TVec3f minusGravity(-mGravity);
    TVec3f starPieceAppearPos(minusGravity * 80.0f);
    starPieceAppearPos += mPosition;

    bool appearedStarPiece;
    if (mAppearThreeStarPiece) {
        appearedStarPiece = MR::appearStarPiece(this, starPieceAppearPos, 3, 10.0f, 40.0f, false);
    } else {
        appearedStarPiece = MR::appearStarPiece(this, starPieceAppearPos, 1, 10.0f, 40.0f, false);
    }

    if (appearedStarPiece) {
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
    }
    kill();
}

void BegomanBaby::control() {
    if (isNerve(GET_NERVE(BegomanBaby, HostTypeNrvPursue)) || isNerve(GET_NERVE(BegomanBaby, HostTypeNrvTurn))) {
        if (incAndCheckTiredCounter()) {
            setNerve(GET_NERVE(BegomanBaby, HostTypeNrvTired));
            return;
        }
    } else {
        mTiredCounter = 0;
    }

    checkTouchElectricRail(isNerve(GET_NERVE(BegomanBaby, HostTypeNrvBlow)));

    if (MR::isStep(this, 1)) {
        if (isNerve(GET_NERVE(BegomanBaby, HostTypeNrvWait)) || isNerve(GET_NERVE(BegomanBaby, HostTypeNrvNoCalcWait))) {
            MR::validateClipping(this);
        } else {
            MR::invalidateClipping(this);
        }
    }

    if (!isNerve(GET_NERVE(BegomanBaby, HostTypeNrvInWater)) && !isNerve(GET_NERVE(BegomanBaby, HostTypeNrvNoCalcWait))) {
        if (isInWaterAndSetWaterNerve(GET_NERVE(BegomanBaby, HostTypeNrvInWater), &mWaterColumnMatrix)) {
            return;
        }
    }

    if (MR::isBindedGroundDamageFire(this)) {
        killWithGenItem();
        MR::emitEffect(this, "Fall");
        return;
    }

    if (MR::isInDeath(this, TVec3f(0.0f, 0.0f, 0.0f))) {
        kill();
        return;
    }

    if (MR::isPressedRoofAndGround(this)) {
        kill();
        return;
    }

    if (isNerve(GET_NERVE(BegomanBaby, HostTypeNrvBlow))) {
        preventSwingby(1.0f);
    }

    if (!isNerve(GET_NERVE(BegomanBaby, HostTypeNrvNoCalcWait)) && !isNerve(GET_NERVE(BegomanBaby, HostTypeNrvWait)) &&
            !isNerve(GET_NERVE(BegomanBaby, HostTypeNrvSignAttack)) && !isNerve(GET_NERVE(BegomanBaby, HostTypeNrvPursue)) &&
            !isNerve(GET_NERVE(BegomanBaby, HostTypeNrvTurn)) && !isNerve(GET_NERVE(BegomanBaby, HostTypeNrvBrake)) &&
            !isNerve(GET_NERVE(BegomanBaby, HostTypeNrvReturn)) && !isNerve(GET_NERVE(BegomanBaby, HostTypeNrvProvoke)) &&
            !isNerve(GET_NERVE(BegomanBaby, HostTypeNrvLaunchTurn)) && !isNerve(GET_NERVE(BegomanBaby, HostTypeNrvKeepDistance)) ||
        !BegomanBase::tryAndSetStarPointerBind(GET_NERVE(BegomanBaby, HostTypeNrvBindStarPointer))) {
        BegomanBase::control();
    }
}

void BegomanBaby::exeNoCalcWait() {
    exeNoCalcWaitCore(0.5f, GET_NERVE(BegomanBaby, HostTypeNrvWait));
}

void BegomanBaby::endNoCalcWait() {
    finishNoCalcWait();
}

void BegomanBaby::exeWait() {
    updateRotateY(0.15f, 0.5f);
    MR::startLevelSound(this, "SE_EM_LV_BABYBEGO_ROT_SLOW");
    exeWaitCore(::hWaitParam, GET_NERVE(BegomanBaby, HostTypeNrvSignAttack), GET_NERVE(BegomanBaby, HostTypeNrvKeepDistance),
                GET_NERVE(BegomanBaby, HostTypeNrvNoCalcWait));
}

void BegomanBaby::exeSignAttack() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "SignAttack", 0);
        MR::startSound(this, "SE_EM_BABYBEGO_PRE_PURSUE");
    }

    updateRotateY(0.5f, 0.5f);
    MR::startLevelSound(this, "SE_EM_LV_BABYBEGO_ROT_MIDDLE");
    return exeSignAttackCore(::hSignAttackParam, GET_NERVE(BegomanBaby, HostTypeNrvPursue));
}

void BegomanBaby::exePursue() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_EM_BABYBEGO_PURSUE_START");
    }
    updateRotateY(0.5f, 0.5f);
    MR::startLevelSound(this, "SE_EM_LV_BEGOMAN_PURSUE");
    exePursueCore(::hPursueParam, GET_NERVE(BegomanBaby, HostTypeNrvBrake), GET_NERVE(BegomanBaby, HostTypeNrvTurn), *getSoundNormal(), 1.0f);
}

void BegomanBaby::exeTurn() {
    updateRotateY(0.4f, 0.5f);
    MR::startLevelSound(this, "SE_EM_LV_BABYBEGO_TURN");
    MR::startLevelSound(this, "SE_EM_LV_BABYBEGO_ROT_MIDDLE");
    exeTurnCore(::hTurnParam, GET_NERVE(BegomanBaby, HostTypeNrvBrake), GET_NERVE(BegomanBaby, HostTypeNrvPursue), false);
}

void BegomanBaby::exeBrake() {
    updateRotateY(0.2f, 0.5f);
    MR::startLevelSound(this, "SE_EM_LV_BEGOMAN_SPARK");
    MR::startLevelSound(this, "SE_EM_LV_BABYBEGO_ROT_MIDDLE");
    exeBrakeCore(GET_NERVE(BegomanBaby, HostTypeNrvTurn));
}

void BegomanBaby::exeStepBack() {
    updateRotateY(0.2f, 0.5f);
    exeStepBackCore(::hWaitParam, GET_NERVE(BegomanBaby, HostTypeNrvWait));
}

void BegomanBaby::exeReturn() {
    updateRotateY(0.2f, 0.5f);
    exeReturnCore(GET_NERVE(BegomanBaby, HostTypeNrvWait));
}

void BegomanBaby::exeProvoke() {
    updateRotateY(0.3f, 0.5f);
    MR::startLevelSound(this, "SE_EM_LV_BABYBEGO_ROT_MIDDLE");
    exeProvokeCore(::hWaitParam, GET_NERVE(BegomanBaby, HostTypeNrvSignAttack));
}

void BegomanBaby::exeTrample() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Trample", nullptr);
        MR::startSound(this, "SE_EM_BABYBEGO_STOMPED");
    }

    updateRotateY(0.25f, 0.5f);
    MR::moveAndTurnToDirection(this, &mFaceVec, mTargetVec, ::hStopParam._0, ::hStopParam._4, ::hStopParam._8, ::hStopParam._C);
    reboundWallAndGround(&mFaceVec, false);
    if (MR::isGreaterStep(this, 180)) {
        setNerve(GET_NERVE(BegomanBaby, HostTypeNrvSignAttack));
    }
}

void BegomanBaby::exeHitReaction() {
    updateRotateY(0.25f, 0.5f);
    exeHitReactionCore(::hHitReactionParam, GET_NERVE(BegomanBaby, HostTypeNrvProvoke));
}

void BegomanBaby::exeTired() {
    if (MR::isFirstStep(this)) {
    }
    updateRotateY(0.2f, 0.5f);
    MR::startLevelSound(this, "SE_EM_LV_BEGOMAN_TURN");
    exeTiredCore(::hTiredParam, GET_NERVE(BegomanBaby, HostTypeNrvWait));
}

void BegomanBaby::exeBlow() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_EM_BEGOMAN_ROT_STOP");
        MR::startBck(this, "Stop", nullptr);
        MR::stopScene(2);
        MR::invalidateExCollisionParts(this);
    }

    updateRotateY(0.3f, 0.5f);
    MR::startLevelSound(this, "SE_EM_LV_BEGOMAN_SPARK");
    MR::startLevelSound(this, "SE_EM_LV_BABYBEGO_ROT_MIDDLE");
    MR::moveAndTurnToDirection(this, &mFaceVec, mTargetVec, ::hHitReactionParam._0, ::hHitReactionParam._4, ::hHitReactionParam._8,
                               ::hHitReactionParam._C);
    reboundWallAndGround(&mFaceVec, false);

    if (MR::isGreaterStep(this, 20)) {
        killWithGenItem();
    }
}

void BegomanBaby::exeAfterLaunch() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Fly", nullptr);
    }

    if (MR::isLessStep(this, 80)) {
        f32 f1 = 5.0f * MR::cos(getNerveStep() * (16 * PI) / 80.0f);
        mVelocity.set(mFaceVec * 5.0f - mGravity * f1);
    } else if (MR::isLessStep(this, 85)) {
        mVelocity.zero();
    } else {
        MR::applyVelocityDampAndGravity(this, 3.0f, 0.8f, 0.98f, 0.98f, 1.0f);
    }

    if (MR::isOnGround(this)) {
        setNerve(GET_NERVE(BegomanBaby, HostTypeNrvAfterLaunchOnGround));
    }
}

void BegomanBaby::exeAfterLaunchOnGround() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Bound", nullptr);
    }
    reboundWallAndGround(&mFaceVec, false);
    MR::applyVelocityDampAndGravity(this, 3.0f, 0.8f, 0.98f, 0.98f, 1.0f);

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(BegomanBaby, HostTypeNrvWait));
    }
}

void BegomanBaby::exeLaunchFromGuarder() {
    exeLaunch();
}

void BegomanBaby::exeLaunchTurn() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Turn", nullptr);
        mFaceVec.set(mTargetVec);
    }

    updateRotateY(0.4f, 0.5f);
    MR::startLevelSound(this, "SE_EM_LV_BABYBEGO_ROT_MIDDLE");
    exeTurnCore(::hTurnParam, GET_NERVE(BegomanBaby, HostTypeNrvBrake), GET_NERVE(BegomanBaby, HostTypeNrvPursue), false);
}

void BegomanBaby::exeInWater() {
    if (MR::isFirstStep(this)) {
        mVelocity += mGravity;
        mVelocity.setLength(10.0f);
        MR::startSound(this, "SE_EM_FALL_INTO_WATER_S");
    }

    if (MR::isGreaterStep(this, 60)) {
        killWithGenItem();
        MR::emitEffect(this, "DeathWater");
        MR::startSound(this, "SE_EM_BEGOMAN_DEAD_IN_WATER");
    }
}

void BegomanBaby::exeKeepDistance() {
    updateRotateY(0.2f, 0.5f);
    exeKeepDistanceCore(GET_NERVE(BegomanBaby, HostTypeNrvWait), GET_NERVE(BegomanBaby, HostTypeNrvSignAttack),
                        GET_NERVE(BegomanBaby, HostTypeNrvBrake), 600.0f, 400.0f);
}

void BegomanBaby::exeBindStarPointer() {
    BegomanBase::exeBindStarPointer();
}

void BegomanBaby::endBindStarPointer() {
    BegomanBase::finishBindStarPointer();
}

void BegomanBaby::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (pSender != getSensor("body")) {
        return;
    }

    if (MR::isSensorEnemy(pReceiver) && MR::isOnGround(this)) {
        MR::sendMsgEnemyAttack(pReceiver, pSender);
        return;
    }

    if (!MR::isSensorPlayer(pReceiver) || isNerve(GET_NERVE(BegomanBaby, HostTypeNrvBlow)) || MR::isPlayerSwingAction() || !MR::isOnGroundPlayer()) {
        return;
    }

    bool msgResult = MR::sendArbitraryMsg(ACTMES_ENEMY_ATTACK_FLIP_ROT, pReceiver, pSender);

    if (msgResult) {
        TVec3f dirFromSenderToReceiver(pSender->mPosition);

        dirFromSenderToReceiver -= pReceiver->mPosition;
        MR::normalizeOrZero(&dirFromSenderToReceiver);

        if (!MR::isNearZero(dirFromSenderToReceiver)) {
            bool reflected = reboundPlaneWithEffect(dirFromSenderToReceiver, 0.0f, 0.0f, "Spark");
            mVelocity += dirFromSenderToReceiver * 2.0f;

            if (reflected) {
                MR::startSound(this, "SE_EM_BABYBEGO_COLLI");
            }
        }
        setNerve(GET_NERVE(BegomanBaby, HostTypeNrvHitReaction));
    }
}

bool BegomanBaby::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

bool BegomanBaby::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (pReceiver != getSensor("body")) {
        return false;
    }

    if (MR::isMsgEnemyAttackElectric(msg)) {
        TVec3f senderVelocity(pSender->mHost->mVelocity);

        MR::vecKillElement(senderVelocity, mGravity, &senderVelocity);
        MR::normalizeOrZero(&senderVelocity);
        return onTouchElectric(pSender->mPosition, senderVelocity);
    }

    if (MR::isMsgExplosionAttack(msg)) {
        if (!isNerve(GET_NERVE(BegomanBaby, HostTypeNrvBlow))) {
            mAppearThreeStarPiece = false;
            calcBlowReaction(pSender->mPosition, pReceiver->mPosition, 35.0f, 15.0f);
            setNerve(GET_NERVE(BegomanBaby, HostTypeNrvBlow));
            return true;
        } else {
            return false;
        }
    }

    if (isNerve(GET_NERVE(BegomanBaby, HostTypeNrvAfterLaunch)) || isNerve(GET_NERVE(BegomanBaby, HostTypeNrvLaunchFromGuarder)) ||
        isNerve(GET_NERVE(BegomanBaby, HostTypeNrvLaunchTurn))) {
        return false;
    }

    if (MR::isSensorEnemy(pSender)) {
        TVec3f dirFromReceiverToSender(pReceiver->mPosition);

        dirFromReceiverToSender -= pSender->mPosition;
        MR::normalizeOrZero(&dirFromReceiverToSender);

        if (MR::isNearZero(dirFromReceiverToSender)) {
            return false;
        }

        bool reflected = reboundPlaneWithEffect(dirFromReceiverToSender, 0.0f, 0.0f, "Spark");

        if (isFallNextMove(150.0f, 150.0f)) {
            setNerve(GET_NERVE(BegomanBaby, HostTypeNrvBrake));
            return true;
        }

        f32 bodyRadius = getSensor("body")->mRadius;
        f32 f1 = pSender->mRadius / bodyRadius;

        MR::addVelocityLimit(this, dirFromReceiverToSender * 4.0f * f1);

        if (reflected) {
            MR::startSound(this, "SE_EM_BABYBEGO_COLLI_BEGOMAN");
        }

        if (!isNerve(GET_NERVE(BegomanBaby, HostTypeNrvTrample)) && !isNerve(GET_NERVE(BegomanBaby, HostTypeNrvProvoke)) &&
            !isNerve(GET_NERVE(BegomanBaby, HostTypeNrvBlow)) && MR::isOnGround(this)) {
            MR::startBck(this, "Turn", nullptr);
            setNerve(GET_NERVE(BegomanBaby, HostTypeNrvTurn));
        }

        return true;
    }
    return false;
}

bool BegomanBaby::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }

    if (MR::isMsgPlayerTrample(msg)) {
        return receiveMsgTrample(pSender, pReceiver);
    }

    if (MR::isMsgPlayerHipDrop(msg)) {
        MR::forceJumpPlayer(-mGravity);
        return true;
    }

    if (pReceiver != getSensor("body")) {
        return false;
    }

    if (isNerve(GET_NERVE(BegomanBaby, HostTypeNrvBlow))) {
        return false;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        mAppearThreeStarPiece = true;
        calcBlowReaction(pSender->mPosition, pReceiver->mPosition, 35.0f, 15.0f);
        setNerve(GET_NERVE(BegomanBaby, HostTypeNrvBlow));
        return true;
    }

    if (MR::isMsgPlayerHitAll(msg)) {
        mAppearThreeStarPiece = false;
        calcBlowReaction(pSender->mPosition, pReceiver->mPosition, 35.0f, 15.0f);
        setNerve(GET_NERVE(BegomanBaby, HostTypeNrvBlow));
        return true;
    }

    return false;
}

bool BegomanBaby::receiveMsgTrample(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(BegomanBaby, HostTypeNrvBlow))) {
        return false;
    }

    if (!pReceiver->isType(ATYPE_BEGOMAN)) {
        return false;
    }

    setNerve(GET_NERVE(BegomanBaby, HostTypeNrvTrample));
    return true;
}

bool BegomanBaby::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return MR::isMsgHitmarkEmit(msg);
}

bool BegomanBaby::onTouchElectric(const TVec3f& rVec1, const TVec3f& rVec2) {
    if (isNerve(GET_NERVE(BegomanBaby, HostTypeNrvBlow)) || isNerve(GET_NERVE(BegomanBaby, HostTypeNrvInWater))) {
        return false;
    }

    if (isNerve(GET_NERVE(BegomanBaby, HostTypeNrvStepBack))) {
        if (!MR::isOnGround(this)) {
            return false;
        }

        kill();
        return true;
    }

    if (BegomanBase::onTouchElectric(rVec1, rVec2)) {
        setNerve(GET_NERVE(BegomanBaby, HostTypeNrvStepBack));
        return true;
    }

    return false;
}

bool BegomanBaby::setNerveReturn() {
    setNerve(GET_NERVE(BegomanBaby, HostTypeNrvReturn));
    return true;
}

const Nerve* BegomanBaby::getNerveWait() {
    return GET_NERVE_ANON(NrvBegomanBaby::HostTypeNrvWait);
}

bool BegomanBaby::calcHeadJoint(TPos3f* pPos, const JointControllerInfo& rInfo) {
    if (isNerve(GET_NERVE(BegomanBaby, HostTypeNrvBlow))) {
        return true;
    }

    TVec3f yDir;
    pPos->getYDir(yDir);

    if (!MR::isSameDirection(yDir, mTargetVec)) {
        MR::makeMtxUpFront(pPos, yDir, mTargetVec);
    }

    return true;
}

void BegomanBaby::calcAndSetBaseMtx() {
    BegomanBase::calcAndSetBaseMtx();
    mBabyDelegator->registerCallBack();
}
