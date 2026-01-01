#include "Game/Enemy/BegomanBaby.hpp"
#include "Game/Util.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"

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
}  // namespace NrvBegomanBaby

namespace {
    const MR::ActorMoveParam hWaitParam = {0.0f, 3.0f, 0.95f, 1.0f};
    const MR::ActorMoveParam hSignAttackParam = {0.0f, 3.0f, 0.8f, 3.0f};
    const MR::ActorMoveParam hPursueParam = {0.4f, 3.0f, 0.97f, 3.0f};
    const MR::ActorMoveParam hTurnParam = {0.4f, 3.0f, 0.95f, 0.0f};
    const MR::ActorMoveParam hStopParam = {0.0f, 3.0f, 0.95f, 0.0f};
    const MR::ActorMoveParam hHitReactionParam = {0.0f, 3.0f, 0.95f, 0.0f};
    const MR::ActorMoveParam hTiredParam = {0.2f, 1.0f, 0.95f, 3.0f};

}  // namespace

BegomanBaby::BegomanBaby(const char* pName) : BegomanBase(pName), mBabyDelegator(nullptr), _134(nullptr), mAppearThreeStarPiece(false) {
    mWaterColumnMatrix.identity();
}

BegomanBaby::BegomanBaby(NameObj* pObj, const char* pName) : BegomanBase(pName), mBabyDelegator(nullptr), _134(pObj), mAppearThreeStarPiece(false) {
    mWaterColumnMatrix.identity();
}

void BegomanBaby::init(const JMapInfoIter& rIter) {
    initCore(rIter, "BegomanBaby", false);
    MR::declareStarPiece(this, 3);

    if (_134 == nullptr) {
        _134 = this;
    }
    initBinder(45.0f, 45.0f, 0);
    initNerve(&NrvBegomanBaby::HostTypeNrvWait::sInstance);
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
    setNerve(&NrvBegomanBaby::HostTypeNrvWait::sInstance);
    MR::validateExCollisionParts(this);
}

void BegomanBaby::appearFromLaunch(const TVec3f& rVec1, const TVec3f& rVec2) {
    BegomanBase::appear();
    MR::emitEffect(this, "Death");
    MR::startSound(this, "SE_EM_BEGOMAN_APPEAR", -1, -1);
    setNerve(&NrvBegomanBaby::HostTypeNrvAfterLaunch::sInstance);

    TVec3f vec(mPosition);

    vec -= rVec1;
    MR::turnVecToPlane(&vec, vec, rVec2);
    MR::normalizeOrZero(&vec);

    mTargetVec.set(vec);
    mFaceVec.set(vec);
}

void BegomanBaby::appearFromGuarder() {
    BegomanBase::appear();
    setNerve(&NrvBegomanBaby::HostTypeNrvLaunchFromGuarder::sInstance);
}

void BegomanBaby::kill() {
    BegomanBase::kill();

    MR::emitEffect(this, "Death");
    MR::startSound(this, "SE_EM_EXPLODE_S", -1, -1);
}

void BegomanBaby::killWithGenItem() {
    TVec3f minusGravity(-mGravity);
    TVec3f starPieceAppearPos(minusGravity.scaleInline(80.0f));
    starPieceAppearPos += mPosition;

    bool appearedStarPiece;
    if (mAppearThreeStarPiece) {
        appearedStarPiece = MR::appearStarPiece(this, starPieceAppearPos, 3, 10.0f, 40.0f, false);
    } else {
        appearedStarPiece = MR::appearStarPiece(this, starPieceAppearPos, 1, 10.0f, 40.0f, false);
    }

    if (appearedStarPiece) {
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
    }
    kill();
}

void BegomanBaby::control() {
    if (isNerve(&NrvBegomanBaby::HostTypeNrvPursue::sInstance) || isNerve(&NrvBegomanBaby::HostTypeNrvTurn::sInstance)) {
        if (incAndCheckTiredCounter()) {
            setNerve(&NrvBegomanBaby::HostTypeNrvTired::sInstance);
            return;
        }
    } else {
        mTiredCounter = 0;
    }

    checkTouchElectricRail(isNerve(&NrvBegomanBaby::HostTypeNrvBlow::sInstance));

    if (MR::isStep(this, 1)) {
        if (isNerve(&NrvBegomanBaby::HostTypeNrvWait::sInstance) || isNerve(&NrvBegomanBaby::HostTypeNrvNoCalcWait::sInstance)) {
            MR::validateClipping(this);
        } else {
            MR::invalidateClipping(this);
        }
    }

    if (!isNerve(&NrvBegomanBaby::HostTypeNrvInWater::sInstance) && !isNerve(&NrvBegomanBaby::HostTypeNrvNoCalcWait::sInstance)) {
        if (isInWaterAndSetWaterNerve(&NrvBegomanBaby::HostTypeNrvInWater::sInstance, &mWaterColumnMatrix)) {
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

    if (isNerve(&NrvBegomanBaby::HostTypeNrvBlow::sInstance)) {
        preventSwingby(1.0f);
    }

    if (!isNerve(&NrvBegomanBaby::HostTypeNrvNoCalcWait::sInstance) && !isNerve(&NrvBegomanBaby::HostTypeNrvWait::sInstance) &&
            !isNerve(&NrvBegomanBaby::HostTypeNrvSignAttack::sInstance) && !isNerve(&NrvBegomanBaby::HostTypeNrvPursue::sInstance) &&
            !isNerve(&NrvBegomanBaby::HostTypeNrvTurn::sInstance) && !isNerve(&NrvBegomanBaby::HostTypeNrvBrake::sInstance) &&
            !isNerve(&NrvBegomanBaby::HostTypeNrvReturn::sInstance) && !isNerve(&NrvBegomanBaby::HostTypeNrvProvoke::sInstance) &&
            !isNerve(&NrvBegomanBaby::HostTypeNrvLaunchTurn::sInstance) && !isNerve(&NrvBegomanBaby::HostTypeNrvKeepDistance::sInstance) ||
        !BegomanBase::tryAndSetStarPointerBind(&NrvBegomanBaby::HostTypeNrvBindStarPointer::sInstance)) {
        BegomanBase::control();
    }
}

void BegomanBaby::exeNoCalcWait() {
    exeNoCalcWaitCore(0.5f, &NrvBegomanBaby::HostTypeNrvWait::sInstance);
}

void BegomanBaby::endNoCalcWait() {
    finishNoCalcWait();
}

void BegomanBaby::exeWait() {
    updateRotateY(0.15f, 0.5f);
    MR::startLevelSound(this, "SE_EM_LV_BABYBEGO_ROT_SLOW", -1, -1, -1);
    exeWaitCore(::hWaitParam, &NrvBegomanBaby::HostTypeNrvSignAttack::sInstance, &NrvBegomanBaby::HostTypeNrvKeepDistance::sInstance,
                &NrvBegomanBaby::HostTypeNrvNoCalcWait::sInstance);
}

void BegomanBaby::exeSignAttack() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "SignAttack", 0);
        MR::startSound(this, "SE_EM_BABYBEGO_PRE_PURSUE", -1, -1);
    }

    updateRotateY(0.5f, 0.5f);
    MR::startLevelSound(this, "SE_EM_LV_BABYBEGO_ROT_MIDDLE", -1, -1, -1);
    return exeSignAttackCore(::hSignAttackParam, &NrvBegomanBaby::HostTypeNrvPursue::sInstance);
}

void BegomanBaby::exePursue() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_EM_BABYBEGO_PURSUE_START", -1, -1);
    }
    updateRotateY(0.5f, 0.5f);
    MR::startLevelSound(this, "SE_EM_LV_BEGOMAN_PURSUE", -1, -1, -1);
    exePursueCore(::hPursueParam, &NrvBegomanBaby::HostTypeNrvBrake::sInstance, &NrvBegomanBaby::HostTypeNrvTurn::sInstance, *getSoundNormal(), 1.0f);
}

void BegomanBaby::exeTurn() {
    updateRotateY(0.4f, 0.5f);
    MR::startLevelSound(this, "SE_EM_LV_BABYBEGO_TURN", -1, -1, -1);
    MR::startLevelSound(this, "SE_EM_LV_BABYBEGO_ROT_MIDDLE", -1, -1, -1);
    exeTurnCore(::hTurnParam, &NrvBegomanBaby::HostTypeNrvBrake::sInstance, &NrvBegomanBaby::HostTypeNrvPursue::sInstance, false);
}

void BegomanBaby::exeBrake() {
    updateRotateY(0.2f, 0.5f);
    MR::startLevelSound(this, "SE_EM_LV_BEGOMAN_SPARK", -1, -1, -1);
    MR::startLevelSound(this, "SE_EM_LV_BABYBEGO_ROT_MIDDLE", -1, -1, -1);
    exeBrakeCore(&NrvBegomanBaby::HostTypeNrvTurn::sInstance);
}

void BegomanBaby::exeStepBack() {
    updateRotateY(0.2f, 0.5f);
    exeStepBackCore(::hWaitParam, &NrvBegomanBaby::HostTypeNrvWait::sInstance);
}

void BegomanBaby::exeReturn() {
    updateRotateY(0.2f, 0.5f);
    exeReturnCore(&NrvBegomanBaby::HostTypeNrvWait::sInstance);
}

void BegomanBaby::exeProvoke() {
    updateRotateY(0.3f, 0.5f);
    MR::startLevelSound(this, "SE_EM_LV_BABYBEGO_ROT_MIDDLE", -1, -1, -1);
    exeProvokeCore(::hWaitParam, &NrvBegomanBaby::HostTypeNrvSignAttack::sInstance);
}

void BegomanBaby::exeTrample() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Trample", nullptr);
        MR::startSound(this, "SE_EM_BABYBEGO_STOMPED", -1, -1);
    }

    updateRotateY(0.25f, 0.5f);
    MR::moveAndTurnToDirection(this, &mFaceVec, mTargetVec, ::hStopParam._0, hStopParam._4, hStopParam._8, hStopParam._C);
    reboundWallAndGround(&mFaceVec, false);
    if (MR::isGreaterStep(this, 180)) {
        setNerve(&NrvBegomanBaby::HostTypeNrvSignAttack::sInstance);
    }
}

void BegomanBaby::exeHitReaction() {
    updateRotateY(0.25f, 0.5f);
    exeHitReactionCore(::hHitReactionParam, &NrvBegomanBaby::HostTypeNrvProvoke::sInstance);
}

void BegomanBaby::exeTired() {
    if (MR::isFirstStep(this)) {
    }
    updateRotateY(0.2f, 0.5f);
    MR::startLevelSound(this, "SE_EM_LV_BEGOMAN_TURN", -1, -1, -1);
    exeTiredCore(::hTiredParam, &NrvBegomanBaby::HostTypeNrvWait::sInstance);
}

void BegomanBaby::exeBlow() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_EM_BEGOMAN_ROT_STOP", -1, -1);
        MR::startBck(this, "Stop", nullptr);
        MR::stopScene(2);
        MR::invalidateExCollisionParts(this);
    }

    updateRotateY(0.3f, 0.5f);
    MR::startLevelSound(this, "SE_EM_LV_BEGOMAN_SPARK", -1, -1, -1);
    MR::startLevelSound(this, "SE_EM_LV_BABYBEGO_ROT_MIDDLE", -1, -1, -1);
    MR::moveAndTurnToDirection(this, &mFaceVec, mTargetVec, ::hHitReactionParam._0, hHitReactionParam._4, hHitReactionParam._8, hHitReactionParam._C);
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
        f32 f1 = 5.0f * JMath::sSinCosTable.cosLapRad(getNerveStep() * (16 * PI) / 80.0f);
        // TODO: fix vector math
        TVec3f scaledGravity(mGravity);
        scaledGravity.scale(f1);
        TVec3f velocity = mFaceVec.scaleInline(5.0f);
        TVec3f velocity2(velocity);
        velocity2 -= scaledGravity;
        mVelocity.set(velocity2);

    } else if (MR::isLessStep(this, 85)) {
        mVelocity.zero();
    } else {
        MR::applyVelocityDampAndGravity(this, 3.0f, 0.8f, 0.98f, 0.98f, 1.0f);
    }

    if (MR::isOnGround(this)) {
        setNerve(&NrvBegomanBaby::HostTypeNrvAfterLaunchOnGround::sInstance);
    }
}

void BegomanBaby::exeAfterLaunchOnGround() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Bound", nullptr);
    }
    reboundWallAndGround(&mFaceVec, false);
    MR::applyVelocityDampAndGravity(this, 3.0f, 0.8f, 0.98f, 0.98f, 1.0f);

    if (MR::isBckStopped(this)) {
        setNerve(&NrvBegomanBaby::HostTypeNrvWait::sInstance);
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
    MR::startLevelSound(this, "SE_EM_LV_BABYBEGO_ROT_MIDDLE", -1, -1, -1);
    exeTurnCore(::hTurnParam, &NrvBegomanBaby::HostTypeNrvBrake::sInstance, &NrvBegomanBaby::HostTypeNrvPursue::sInstance, false);
}

void BegomanBaby::exeInWater() {
    if (MR::isFirstStep(this)) {
        mVelocity += mGravity;
        mVelocity.setLength(10.0f);
        MR::startSound(this, "SE_EM_FALL_INTO_WATER_S", -1, -1);
    }

    if (MR::isGreaterStep(this, 60)) {
        killWithGenItem();
        MR::emitEffect(this, "DeathWater");
        MR::startSound(this, "SE_EM_BEGOMAN_DEAD_IN_WATER", -1, -1);
    }
}

void BegomanBaby::exeKeepDistance() {
    updateRotateY(0.2f, 0.5f);
    exeKeepDistanceCore(&NrvBegomanBaby::HostTypeNrvWait::sInstance, &NrvBegomanBaby::HostTypeNrvSignAttack::sInstance,
                        &NrvBegomanBaby::HostTypeNrvBrake::sInstance, 600.0f, 400.0f);
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

    if (!MR::isSensorPlayer(pReceiver) || isNerve(&NrvBegomanBaby::HostTypeNrvBlow::sInstance) || MR::isPlayerSwingAction() ||
        !MR::isOnGroundPlayer()) {
        return;
    }

    bool msgResult = MR::sendArbitraryMsg(ACTMES_ENEMY_ATTACK_FLIP_ROT, pReceiver, pSender);

    if (msgResult) {
        TVec3f dirFromSenderToReceiver(pSender->mPosition);

        dirFromSenderToReceiver -= pReceiver->mPosition;
        MR::normalizeOrZero(&dirFromSenderToReceiver);

        if (!MR::isNearZero(dirFromSenderToReceiver)) {
            bool reflected = reboundPlaneWithEffect(dirFromSenderToReceiver, 0.0f, 0.0f, "Spark");
            mVelocity += dirFromSenderToReceiver.scaleInline(2.0f);

            if (reflected) {
                MR::startSound(this, "SE_EM_BABYBEGO_COLLI", -1, -1);
            }
        }
        setNerve(&NrvBegomanBaby::HostTypeNrvHitReaction::sInstance);
    }
}

bool BegomanBaby::receiveMsgPush(HitSensor*, HitSensor*) {
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

        if (!isNerve(&NrvBegomanBaby::HostTypeNrvBlow::sInstance)) {
            mAppearThreeStarPiece = false;
            calcBlowReaction(pSender->mPosition, pReceiver->mPosition, 35.0f, 15.0f);
            setNerve(&NrvBegomanBaby::HostTypeNrvBlow::sInstance);
            return true;
        } else {
            return false;
        }
    }

    if (isNerve(&NrvBegomanBaby::HostTypeNrvAfterLaunch::sInstance) || isNerve(&NrvBegomanBaby::HostTypeNrvLaunchFromGuarder::sInstance) ||
        isNerve(&NrvBegomanBaby::HostTypeNrvLaunchTurn::sInstance)) {
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
            setNerve(&NrvBegomanBaby::HostTypeNrvBrake::sInstance);
            return true;
        }

        f32 bodyRadius = getSensor("body")->mRadius;
        f32 f1 = pSender->mRadius / bodyRadius;

        TVec3f vec2 = TVec3f(dirFromReceiverToSender.scaleInline(4.0f));
        vec2.scale(f1);

        MR::addVelocityLimit(this, vec2);

        if (reflected) {
            MR::startSound(this, "SE_EM_BABYBEGO_COLLI_BEGOMAN", -1, -1);
        }

        if (!isNerve(&NrvBegomanBaby::HostTypeNrvTrample::sInstance) && !isNerve(&NrvBegomanBaby::HostTypeNrvProvoke::sInstance) &&
            !isNerve(&NrvBegomanBaby::HostTypeNrvBlow::sInstance) && MR::isOnGround(this)) {
            MR::startBck(this, "Turn", nullptr);
            setNerve(&NrvBegomanBaby::HostTypeNrvTurn::sInstance);
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

    if (isNerve(&NrvBegomanBaby::HostTypeNrvBlow::sInstance)) {
        return false;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        mAppearThreeStarPiece = true;
        calcBlowReaction(pSender->mPosition, pReceiver->mPosition, 35.0f, 15.0f);
        setNerve(&NrvBegomanBaby::HostTypeNrvBlow::sInstance);
        return true;
    }

    if (MR::isMsgPlayerHitAll(msg)) {
        mAppearThreeStarPiece = false;
        calcBlowReaction(pSender->mPosition, pReceiver->mPosition, 35.0f, 15.0f);
        setNerve(&NrvBegomanBaby::HostTypeNrvBlow::sInstance);
        return true;
    }

    return false;
}

bool BegomanBaby::receiveMsgTrample(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvBegomanBaby::HostTypeNrvBlow::sInstance)) {
        return false;
    }

    if (!pReceiver->isType(ATYPE_BEGOMAN)) {
        return false;
    }

    setNerve(&NrvBegomanBaby::HostTypeNrvTrample::sInstance);
    return true;
}

bool BegomanBaby::receiveOtherMsg(u32 msg, HitSensor*, HitSensor*) {
    return MR::isMsgHitmarkEmit(msg);
}

bool BegomanBaby::onTouchElectric(const TVec3f& rVec1, const TVec3f& rVec2) {
    if (isNerve(&NrvBegomanBaby::HostTypeNrvBlow::sInstance) || isNerve(&NrvBegomanBaby::HostTypeNrvInWater::sInstance)) {
        return false;
    }

    if (isNerve(&NrvBegomanBaby::HostTypeNrvStepBack::sInstance)) {
        if (!MR::isOnGround(this)) {
            return false;
        }

        kill();
        return true;
    }

    if (BegomanBase::onTouchElectric(rVec1, rVec2)) {
        setNerve(&NrvBegomanBaby::HostTypeNrvStepBack::sInstance);
        return true;
    }

    return false;
}

bool BegomanBaby::setNerveReturn() {
    setNerve(&NrvBegomanBaby::HostTypeNrvReturn::sInstance);
    return true;
}

const Nerve* BegomanBaby::getNerveWait() {
    return &::NrvBegomanBaby::HostTypeNrvWait::sInstance;
}

bool BegomanBaby::calcHeadJoint(TPos3f* pPos, const JointControllerInfo& rInfo) {
    if (isNerve(&NrvBegomanBaby::HostTypeNrvBlow::sInstance)) {
        return true;
    }

    TVec3f yDir;
    pPos->getYDirInline(yDir);

    if (!MR::isSameDirection(yDir, mTargetVec, 0.01f)) {
        MR::makeMtxUpFront(pPos, yDir, mTargetVec);
    }
    
    return true;
}

void BegomanBaby::calcAndSetBaseMtx() {
    BegomanBase::calcAndSetBaseMtx();
    mBabyDelegator->registerCallBack();
}
