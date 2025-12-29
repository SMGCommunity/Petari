#include "Game/Enemy/BegomanBase.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Map/WaterInfo.hpp"
#include "Game/Util.hpp"
#include "Game/mapObj/ElectricRailHolder.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"
#include "revolution/types.h"

namespace NrvBegomanAttackPermitter {
    NEW_NERVE(HostTypeNrvWait, BegomanAttackPermitter, Wait);
    NEW_NERVE(HostTypeNrvReceive, BegomanAttackPermitter, Receive);
    NEW_NERVE(HostTypeNrvPermit, BegomanAttackPermitter, Permit);

}  // namespace NrvBegomanAttackPermitter

namespace {
    // unused
    static const BegomanSound sound_baby = {"SE_EM_LV_BABYBEGO_DASH"};

    static const BegomanSound sound_normal = {"SE_EM_LV_BEGOMAN_DASH"};
    static const BegomanSound sound_boss = {"SE_BM_LV_BBEGO_DASH"};

    const MR::ActorMoveParam hReturnWaitParam = {0.0f, 1.0f, 0.95f, 0.0f};
    const MR::ActorMoveParam hReturnMoveParam = {0.2f, 1.0f, 0.95f, 3.0f};
    const MR::ActorMoveParam hBrakeParam = {0.0f, 1.0f, 0.7f, 3.0f};
    const MR::ActorMoveParam hBrakeOnAirParam = {0.0f, 1.0f, 0.95f, 3.0f};
    const MR::ActorMoveParam hKeepDistanceFarParam = {0.35f, 1.0f, 0.95f, 3.0f};
    const MR::ActorMoveParam hKeepDistanceNearParam = {-0.22f, 1.0f, 0.95f, 3.0f};
    const MR::ActorMoveParam hKeepDistanceWaitParam = {0.0f, 1.0f, 0.95f, 3.0f};

    const f32 hTurnStartDegree = 3.0f;
    const f32 hTurnEndDegree = 10.0f;
    const f32 hIsFaceToPlayerDegree = 30.0f;
}  // namespace

BegomanBase::BegomanBase(const char* pName)
    : LiveActor(pName), mBaseDelegator(nullptr), mFaceVec(0.0f, 0.0f, 1.0f), mTargetVec(0.0f, 0.0f, 1.0f), _A8(0.0f, 0.0f, -1.0f), _B4(1.0f, 1.0f, 1.0f),
      _C0(0, 0, 0, 1), _D0(0, 0, 0, 1), mTiredCounter(0), mElectricCounter(0), mInitPos(0.0f, 0.0f, 0.0f), mScaleControler(nullptr),
      mStarPointBind(nullptr), mCanTrySetReturn(false) {}

// needed to get a string to show up in .data, should be deadstripped.
const BegomanSound* BegomanBase::getSoundBaby() {
    return &::sound_baby;
}

const BegomanSound* BegomanBase::getSoundNormal() {
    return &::sound_normal;
}

const BegomanSound* BegomanBase::getSoundBoss() {
    return &::sound_boss;
}

void BegomanBase::initCore(const JMapInfoIter& rIter, const char* pModelArcName, bool a1) {
    MR::createSceneObj(0x3D);
    MR::useStageSwitchWriteDead(this, rIter);
    MR::useStageSwitchReadA(this, rIter);
    initModelManagerWithAnm(pModelArcName, nullptr, false);

    if (MR::isValidInfo(rIter)) {
        MR::initDefaultPos(this, rIter);
        mInitPos.set(mPosition);
        MR::setGroupClipping(this, rIter, 0x10);
        s32 arg0 = -1;
        MR::getJMapInfoArg0NoInit(rIter, &arg0);
        if (arg0 == -1) {
            mCanTrySetReturn = false;
        } else {
            mCanTrySetReturn = true;
        }
    }
    MR::connectToSceneEnemy(this);

    mBaseDelegator = MR::createJointDelegatorWithNullChildFunc(this, &BegomanBase::calcJointLocator1, "Locator1");
    MR::onCalcGravity(this);
    MR::initLightCtrl(this);

    // this function should be inlined
    MR::makeQuatAndFrontFromRotate(&_C0, &mFaceVec, this);
    mRotation.set(0.0f);
}

void BegomanBase::initEffectAndStarPointerBind() {
    initEffect(1);
    MR::startAction(this, "Wait");
    calcAnim();

    f32 modelBoundingRadius;
    MR::calcModelBoundingRadius(&modelBoundingRadius, this);

    // float regswaps here
    f32 half = 0.5f;
    modelBoundingRadius *= 0.75f;
    MR::initStarPointerTarget(this, 0.0f, TVec3f(0.0f, half * modelBoundingRadius, 0.0f));

    mScaleControler = new AnimScaleController(nullptr);
    mScaleControler->setParamTight();
    mStarPointBind = new WalkerStateBindStarPointer(this, mScaleControler);
}

void BegomanBase::initAfterPlacement() {
    MR::trySetMoveLimitCollision(this);
}

void BegomanBase::initShadow(f32 shadowRadius, const char* pShadowName) {
    MR::initShadowVolumeSphere(this, shadowRadius);

    TVec3f zeroVec = TVec3f(0.0f, 0.0f, 0.0f);
    MR::setShadowDropPositionAtJoint(this, nullptr, pShadowName, &zeroVec);
}

void BegomanBase::initEffect(s32 a1) {
    initEffectKeeper(a1 + 3, nullptr, false);

    MR::addEffect(this, "BegomanSpark");
    MR::addEffect(this, "BegomanHit");
    MR::addEffect(this, "BegomanFailureHit");

    MR::setEffectHostSRT(this, "BegomanSpark", nullptr, nullptr, nullptr);
    MR::setEffectHostSRT(this, "BegomanHit", nullptr, nullptr, nullptr);
    MR::setEffectHostSRT(this, "BegomanFailureHit", nullptr, nullptr, nullptr);

    MR::setEffectName(this, "BegomanSpark", "Spark");
    MR::setEffectName(this, "BegomanHit", "Hit");
    MR::setEffectName(this, "BegomanFailureHit", "FailureHit");
}

void BegomanBase::initSensor(s32 numBaseSensors, f32 f1, f32 f2, const char* sensorJointName) {
    f32 yScale = mScale.y;
    initHitSensor(numBaseSensors + 3);

    MR::addHitSensor(this, "check", 0x7F, 1, f1 * yScale, TVec3f(0.0f, 0.5f * f1 * yScale, 1.5f * f1 * yScale));
    MR::addHitSensorAtJoint(this, "body", sensorJointName, 0x1D, 0x20, f1 * yScale, TVec3f(0.0f, 0.5f * f1 * yScale, 0.0f));
    MR::addHitSensorAtJoint(this, "pushed_rail", sensorJointName, 0x7F, 0x20, f2 * yScale, TVec3f(0.0f, 0.5f * f2 * yScale, 0.0f));
}

void BegomanBase::initUseSwitchB(const JMapInfoIter& rIter, const MR::FunctorBase& rFunctor) {
    if (MR::isValidInfo(rIter) && MR::useStageSwitchReadB(this, rIter)) {
        MR::listenStageSwitchOnB(this, rFunctor);
    }
}

void BegomanBase::makeActorDead() {
    LiveActor::makeActorDead();
}

void BegomanBase::appear() {
    LiveActor::appear();
    MR::onBind(this);
    MR::onCalcGravity(this);
    getSensor("body")->validate();
}

void BegomanBase::kill() {
    LiveActor::kill();
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
}

void BegomanBase::control() {
    if (mScaleControler != nullptr) {
        mScaleControler->updateNerve();
    }

    updateTargetVec();

    TVec3f upVec;
    MR::calcUpVec(&upVec, this);

    TVec3f blendVec;
    TQuat4f yRotationQuat;
    TVec3f minusGravity = -mGravity;

    MR::vecBlend(upVec, minusGravity, &blendVec, 0.1f);
    MR::normalizeOrZero(&blendVec);

    if (MR::isNearZero(blendVec)) {
        blendVec.set(upVec);
    }

    MR::makeQuatUpFront(&_C0, blendVec, mFaceVec);
    _A8.set(upVec);

    yRotationQuat.setRotate(0.0f, 1.0f, 0.0f, mRotation.y);
    PSQUATMultiply(&yRotationQuat, &_D0, &_D0);
}

void BegomanBase::startClipped() {
    LiveActor::startClipped();
}

void BegomanBase::exeNoCalcWaitCore(f32 f1, const Nerve* pNerve) {
    if (MR::isFirstStep(this)) {
        MR::offBind(this);
        MR::offCalcGravity(this);
        MR::validateClipping(this);
    }

    updateRotateY(0.15f, f1);
    // has to be zero because it sets z first.
    mVelocity.zero();
    // incorrectly inlined
    TVec3f playerPos(*MR::getPlayerPos());
    playerPos.sub(mPosition);
    f32 f2 = playerPos.squared();

    if (pNerve != nullptr) {
        if (MR::calcDistanceToPlayer(mPosition) < 1000.0f) {
            if (!MR::isValidSwitchA(this) || MR::isOnSwitchA(this)) {
                MR::onBind(this);
                MR::onCalcGravity(this);
                setNerve(pNerve);
            }
        }
    }
}

void BegomanBase::finishNoCalcWait() {
    MR::onBind(this);
    MR::onCalcGravity(this);
    MR::invalidateClipping(this);
}

void BegomanBase::exeWaitCore(const MR::ActorMoveParam& rMoveParam, const Nerve* pNerve1, const Nerve* pNerve2, const Nerve* pNerve3) {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Wait");
    }

    MR::moveAndTurnToDirection(this, &mFaceVec, mTargetVec, rMoveParam._0, rMoveParam._4, rMoveParam._8, rMoveParam._C);
    reboundWallAndGround(&mFaceVec, false);

    if (!isNearInitPos()) {
        if (trySetReturnNerve()) {
            return;
        }
    }
    if (MR::calcDistanceToPlayer(mPosition) < 1000.0f) {
        if (MR::isValidSwitchA(this)) {
            if (!MR::isOnSwitchA(this)) {
                return;
            }
        }

        if (!isFallNextMove(150.0f, 150.0f)) {
            TVec3f touchRailVec;

            if (!ElectricRailFunction::isTouchRail(getSensor("check"), &touchRailVec, nullptr)) {
                if (requestAttack()) {
                    setNerve(pNerve1);
                    return;
                } else {
                    setNerve(pNerve2);
                    return;
                }
            }

            TVec3f touchRailToSensor(touchRailVec);
            touchRailToSensor.sub(getSensor("check")->mPosition);
            MR::normalizeOrZero(&touchRailToSensor);

            if (!MR::isNearZero(touchRailToSensor)) {
                if (!(0.5f < touchRailToSensor.dot(mFaceVec))) {
                    if (requestAttack()) {
                        setNerve(pNerve1);
                    } else {
                        setNerve(pNerve2);
                    }
                }
            }
        }
    } else {
        if (MR::isOnGround(this)) {
            setNerve(pNerve3);
        }
    }
}

void BegomanBase::exeSignAttackCore(const MR::ActorMoveParam& rMoveParam, const Nerve* pNerve) {
    MR::moveAndTurnToDirection(this, &mFaceVec, mTargetVec, rMoveParam._0, rMoveParam._4, rMoveParam._8, rMoveParam._C);
    reboundWallAndGround(&mFaceVec, false);

    if (!requestAttack()) {
        if (!trySetReturnNerve()) {
            setNerve(getNerveWait());
        }
    } else {
        if (MR::isActionEnd(this)) {
            setNerve(pNerve);
        }
    }
}

void BegomanBase::exePursueCore(const MR::ActorMoveParam& rMoveParam, const Nerve* pNerve1, const Nerve* pNerve2, const BegomanSound& rSound,
                                f32 f1) {
    if (MR::isStep(this, 10)) {
        MR::startAction(this, "Attack");
    }

    if (MR::isGreaterEqualStep(this, 10)) {
        MR::startLevelSound(this, rSound.mSound, -1, -1, -1);
    }

    if (MR::isStep(this, 18)) {
        mVelocity = mFaceVec.scaleInline(15.0f).scaleInline(f1);
    }

    if (isFallNextMove(150.0f, 150.0f)) {
        setNerve(pNerve1);
        return;
    }
    MR::moveAndTurnToDirection(this, &mFaceVec, mTargetVec, rMoveParam._0 * f1, rMoveParam._4, rMoveParam._8, rMoveParam._C);

    if (MR::isGreaterStep(this, 18)) {
        reboundWallAndGround(&mFaceVec, false);
    } else {
        reboundWallAndGround(&mFaceVec, true);
    }

    if (!requestAttack()) {
        if (!trySetReturnNerve()) {
            setNerve(getNerveWait());
        }
        return;
    }

    if (ElectricRailFunction::isTouchRail(getSensor("check"), nullptr, nullptr)) {
        setNerve(pNerve1);
        return;
    }

    if (!MR::isFaceToPlayerHorizontalDegree(this, mFaceVec, ::hIsFaceToPlayerDegree)) {
        MR::startAction(this, "Turn");
        setNerve(pNerve2);
        return;
    }

    if (MR::isGreaterStep(this, 180)) {
        MR::turnDirection(this, &mFaceVec, mTargetVec, 3.0f);
        MR::startAction(this, "Turn");
        setNerve(pNerve2);
    }

    if (!trySetReturnNerve()) {
        if (2000.0f < MR::calcDistanceToPlayer(mPosition) && !trySetReturnNerve()) {
            setNerve(getNerveWait());
        }
    }
}

void BegomanBase::exeTurnCore(const MR::ActorMoveParam& rMoveParam, const Nerve* pNerve1, const Nerve* pNerve2, bool notCheckRail) {
    if (MR::isOnGround(this) && isFallNextMove(150.0f, 150.0f)) {
        setNerve(pNerve1);
        return;
    }

    f32 nerveValue = MR::calcNerveValue(this, 0x3c, ::hTurnStartDegree, ::hTurnEndDegree);
    MR::moveAndTurnToDirection(this, &mFaceVec, mTargetVec, rMoveParam._0, rMoveParam._4, rMoveParam._8, nerveValue);
    reboundWallAndGround(&mFaceVec, true);

    if (!notCheckRail) {
        if (ElectricRailFunction::isTouchRail(getSensor("check"), nullptr, nullptr)) {
            setNerve(pNerve1);
            return;
        }
    }

    if (!trySetReturnNerve()) {
        bool attackRequested = requestAttack();

        if (!attackRequested && MR::isGreaterStep(this, 30)) {
            if (!trySetReturnNerve()) {
                setNerve(getNerveWait());
            }
            return;
        }

        if (MR::isGreaterStep(this, 60)) {
            if (attackRequested && MR::isFaceToPlayerHorizontalDegree(this, mFaceVec, ::hIsFaceToPlayerDegree)) {
                setNerve(pNerve2);
                return;
            }

            if (2000.0f < MR::calcDistanceToPlayer(mPosition) && !trySetReturnNerve()) {
                setNerve(getNerveWait());
            }
        }
    }
}

void BegomanBase::exeBrakeCore(const Nerve* pNerve) {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Brake");
        MR::vecKillElement(mFaceVec, mVelocity, &mFaceVec);
    }

    if (MR::isOnGround(this)) {
        MR::moveAndTurnToDirection(this, &mFaceVec, mTargetVec, ::hBrakeParam._0, ::hBrakeParam._4, ::hBrakeParam._8, ::hBrakeParam._C);
    } else {
        MR::moveAndTurnToDirection(this, &mFaceVec, mTargetVec, ::hBrakeOnAirParam._0, ::hBrakeOnAirParam._4, ::hBrakeOnAirParam._8,
                                   ::hBrakeOnAirParam._C);
    }

    reboundWallAndGround(&mFaceVec, true);

    if (isFallNextMove(150.0f, 150.0f)) {
        MR::addVelocityLimit(this, (-mFaceVec).scaleInline(0.0f));
    } else {
        if (!ElectricRailFunction::isTouchRail(getSensor("check"), nullptr, nullptr) && MR::isGreaterStep(this, 50)) {
            MR::startAction(this, "Brake");
            setNerve(pNerve);
        }
    }
}

void BegomanBase::exeStepBackCore(const MR::ActorMoveParam& rMoveParam, const Nerve* pNerve) {
    // unused check for first step
    if (MR::isFirstStep(this)) {
    }

    MR::moveAndTurnToDirection(this, &mFaceVec, mTargetVec, rMoveParam._0, rMoveParam._4, rMoveParam._8, rMoveParam._C);
    if (MR::isOnGround(this)) {
        setNerve(pNerve);
    }
}

void BegomanBase::exeProvokeCore(const MR::ActorMoveParam& rMoveParam, const Nerve* pNerve) {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Provoke");
    }

    MR::moveAndTurnToDirection(this, &mFaceVec, mTargetVec, rMoveParam._0, rMoveParam._4, rMoveParam._8, rMoveParam._C);
    reboundWallAndGround(&mFaceVec, false);

    TVec3f* gravity;
    gravity = &mGravity;
    mVelocity.scale(gravity->dot(mVelocity), *gravity);

    if (MR::isGreaterStep(this, 120)) {
        setNerve(pNerve);
    }
}

void BegomanBase::exeHitReactionCore(const MR::ActorMoveParam& rMoveParam, const Nerve* pNerve) {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "HitReaction");
    }

    MR::moveAndTurnToDirection(this, &mFaceVec, mTargetVec, rMoveParam._0, rMoveParam._4, rMoveParam._8, rMoveParam._C);
    reboundWallAndGround(&mFaceVec, false);

    if (MR::isActionEnd(this)) {
        setNerve(pNerve);
    }
}

void BegomanBase::exeTiredCore(const MR::ActorMoveParam& rMoveParam, const Nerve* pNerve) {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Tired");
    }

    MR::moveAndTurnToDirection(this, &mFaceVec, mTargetVec, rMoveParam._0, rMoveParam._4, rMoveParam._8, rMoveParam._C);
    reboundWallAndGround(&mFaceVec, false);

    if (MR::isGreaterStep(this, 120)) {
        setNerve(pNerve);
    }
}

void BegomanBase::exeReturnCore(const Nerve* pNerve) {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Brake");
    }

    if (MR::isLessStep(this, 180)) {
        MR::moveAndTurnToTarget(this, &mFaceVec, mInitPos, ::hReturnWaitParam._0, ::hReturnWaitParam._4, ::hReturnWaitParam._8,
                                ::hReturnWaitParam._C);
    } else {
        MR::moveAndTurnToTarget(this, &mFaceVec, mInitPos, ::hReturnMoveParam._0, ::hReturnMoveParam._4, ::hReturnMoveParam._8,
                                ::hReturnMoveParam._C);
    }

    reboundWallAndGround(&mFaceVec, false);
    if (isNearInitPos()) {
        setNerve(pNerve);
    }
}

void BegomanBase::exeKeepDistanceCore(const Nerve* pNerve1, const Nerve* pNerve2, const Nerve* pNerve3, f32 farDist, f32 nearDist) {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Brake");
    }

    f32 distToPlayer = MR::calcDistanceToPlayer(mPosition);
    if (1200.0f < distToPlayer) {
        setNerve(pNerve1);
        return;
    }
    if (farDist < distToPlayer) {
        MR::moveAndTurnToPlayer(this, &mFaceVec, ::hKeepDistanceFarParam._0, ::hKeepDistanceFarParam._4, ::hKeepDistanceFarParam._8,
                                ::hKeepDistanceFarParam._C);
    } else if (distToPlayer < nearDist) {
        MR::moveAndTurnToPlayer(this, &mFaceVec, ::hKeepDistanceNearParam._0, ::hKeepDistanceNearParam._4, ::hKeepDistanceNearParam._8,
                                ::hKeepDistanceNearParam._C);
        addVelocityEscapeToSide(0.6f);
    } else {
        MR::moveAndTurnToPlayer(this, &mFaceVec, ::hKeepDistanceWaitParam._0, ::hKeepDistanceWaitParam._4, ::hKeepDistanceWaitParam._8,
                                ::hKeepDistanceWaitParam._C);
    }

    if (isFallNextMove(150.0f, 150.0f)) {
        setNerve(pNerve3);
        return;
    }

    reboundWallAndGround(&mFaceVec, false);
    if (requestAttack()) {
        setNerve(pNerve2);
        return;
    }

    trySetReturnNerve();
}

void BegomanBase::addVelocityEscapeToSide(f32 velocity) {
    TVec3f sideVec;
    MR::calcSideVec(&sideVec, this);

    if (mInvertSideVec) {
        sideVec.scale(-1.0f);
    }

    if (0.0f < mTargetVec.dot(sideVec) || 0.0f < sideVec.dot(*MR::getPlayerVelocity())) {
        mInvertSideVec = !mInvertSideVec;
    }

    sideVec.setLength(velocity);
    MR::addVelocity(this, sideVec);
}

void BegomanBase::exeLaunch() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensors(this);
    }

    MR::applyVelocityDampAndGravity(this, 0.98f, 0.98f, 0.98f, 0.98f, 1.0f);

    if (MR::isOnGround(this)) {
        MR::validateHitSensors(this);
        setNerve(getNerveWait());
    }
}

void BegomanBase::exeBindStarPointer() {
    if (MR::updateActorStateAndNextNerve(this, mStarPointBind, getNerveWait())) {
        return;
    }
}

void BegomanBase::finishBindStarPointer() {
    mStarPointBind->kill();
}

bool BegomanBase::tryAndSetStarPointerBind(const Nerve* pNerve) {
    if (mStarPointBind->tryStartPointBind()) {
        setNerve(pNerve);
        return true;
    }
    return false;
}

bool BegomanBase::isNearInitPos() const {
    return mInitPos.squared(mPosition) < 40000.0f;
}

bool BegomanBase::trySetReturnNerve() {
    if (MR::isValidSwitchA(this) && !MR::isOnSwitchA(this) && mCanTrySetReturn) {
        setNerveReturn();
        return true;
    }
    return false;
}

bool BegomanBase::incAndCheckTiredCounter() {
    if (mTiredCounter < 180) {
        mTiredCounter++;
    }
    return mTiredCounter == 180;
}

void BegomanBase::launchBegomanCore(LiveActor* pActor, BegomanBase** begomanArray, s32 numBegoman, f32 distFromLauncher, f32 f2, f32 f3,
                                    const TVec3f* pVec) {
    TVec3f vec2;
    TVec3f vec1;
    if (pVec == nullptr) {
        MR::calcFrontVec(&vec1, pActor);
        MR::calcSideVec(&vec2, pActor);
    } else {
        vec1.set(*pVec);
        PSVECCrossProduct(&pActor->mGravity, &vec1, &vec2);
        PSVECCrossProduct(&vec2, &pActor->mGravity, &vec1);
    }

    f32 angle = 0.0f;

    for (int i = 0; i < numBegoman; i++) {
        if (!MR::isDead(begomanArray[i])) {
            continue;
        }
        TVec3f directionFromLauncher(vec2);
        directionFromLauncher.scale(JMath::sSinCosTable.cosLapRad(angle));

        directionFromLauncher.add(vec1.scaleInline(JMath::sSinCosTable.sinLapRad(angle)));

        begomanArray[i]->mPosition.set(pActor->mPosition.addOperatorInLine(directionFromLauncher.scaleInline(distFromLauncher)));
        begomanArray[i]->mVelocity.set(directionFromLauncher.scaleInline(f2).subOperatorInLine(pActor->mGravity.scaleInline(f3)));
        begomanArray[i]->mFaceVec.set(directionFromLauncher);

        angle += TWO_PI / (numBegoman);
    }
}

void BegomanBase::launchBegoman(LiveActor* pActor, BegomanBase** begomanArray, s32 numBegoman, f32 distFromLauncher, f32 f2, f32 f3,
                                const TVec3f* pVec) {
    launchBegomanCore(pActor, begomanArray, numBegoman, distFromLauncher, f2, f3, pVec);

    for (int i = 0; i < numBegoman; i++) {
        begomanArray[i]->appear();
        begomanArray[i]->setNerveLaunch();
    }
}

void BegomanBase::updateRotateY(f32 newRotationTarget, f32 rotationLimit) {
    f32 newYRotation = 0.0f;

    if (0.0f < newRotationTarget - mRotation.y) {
        if (rotationLimit + mRotation.y >= newRotationTarget) {
            newYRotation = newRotationTarget;
        } else {
            newYRotation = 0.0f;
        }

        mRotation.y = newYRotation;
    } else {
        if (mRotation.y - rotationLimit >= newRotationTarget) {
            newYRotation = 0.0f;
        } else {
            newYRotation = newRotationTarget;
        }

        mRotation.y = newYRotation;
    }
}

void BegomanBase::updateTargetVec() {
    mTargetVec.set(*MR::getPlayerCenterPos());
    mTargetVec.sub(mPosition);
    MR::turnDirectionToGround(this, &mTargetVec);
}

bool BegomanBase::reboundPlaneWithEffect(const TVec3f& rVec, f32 f1, f32 f2, const char* pEffectName) {
    bool reflected = MR::calcReflectionVector(&mVelocity, rVec, f1, f2);

    if (pEffectName != nullptr && reflected) {
        TVec3f bodySensorPos(getSensor("body")->mPosition);

        bodySensorPos.sub(rVec.scaleInline(getSensor("body")->mRadius));
        MR::emitEffectHit(this, bodySensorPos, pEffectName);
    }
    return reflected;
}

void BegomanBase::reboundWallAndGround(TVec3f* pOut, bool emitEffect) {
    if (MR::isOnGround(this)) {
        TVec3f groundNormal(*MR::getGroundNormal(this));

        if (emitEffect) {
            MR::calcReflectionVector(&mVelocity, groundNormal, 0.0f, 0.0f);
            MR::emitEffect(this, "OnGroundSpark");
        } else {
            MR::calcReflectionVector(&mVelocity, groundNormal, 0.5, 100.0);
            MR::deleteEffect(this, "OnGroundSpark");
        }
    } else {
        MR::deleteEffect(this, "OnGroundSpark");
    }

    if (MR::isBindedWallOfMap(this)) {
        TVec3f wallNormal(*MR::getWallNormal(this));
        bool reboundWall = reboundPlaneWithEffect(wallNormal, 0.0f, 0.0f, "Spark");
        mVelocity.add(wallNormal.scaleInline(5.0f));

        if (reboundWall) {
            MR::startLevelSound(this, "SE_EM_LV_BEGOMAN_COLLI_WALL", -1, -1, -1);
        }

        if (pOut != nullptr) {
            if (!MR::isNearZero(mVelocity)) {
                pOut->set(mVelocity);
                MR::normalize(pOut);
            }
        }
    }
}

bool BegomanBase::isFallNextMove(f32 f1, f32 f2) {
    TVec3f* velocityCopy = &mVelocity;
    TVec3f* GravityCopy = &mGravity;

    f32 dot = GravityCopy->dot(*velocityCopy);
    TVec3f ScaleAddResult;

    JMAVECScaleAdd(GravityCopy, velocityCopy, &ScaleAddResult, -dot);

    if (MR::isNearZero(ScaleAddResult)) {
        f32 scaledBinderRadius = MR::getBinderRadius(this);
        scaledBinderRadius = scaledBinderRadius * mScale.y * 2.0f;
        return MR::isFallNextMove(mPosition, mFaceVec, mGravity, f1, scaledBinderRadius, f2, nullptr);
    } else {
        f32 scaledBinderRadius = MR::getBinderRadius(this);
        scaledBinderRadius = scaledBinderRadius * mScale.y * 2.0f;
        return MR::isFallNextMove(this, f1, scaledBinderRadius, f2, nullptr);
    }
}

f32 BegomanBase::getRailPushVelHBoss() const {
    return 30.0f;
}

f32 BegomanBase::getRailPushJumpBoss() const {
    return 30.0f;
}

void BegomanBase::calcDirectionAwayFromRail(TVec3f* pOut, const TVec3f& rVec1, const TVec3f& rVec2) {
    TVec3f vec1(rVec2);

    if (MR::isNearZero(vec1)) {
        vec1.set(-mFaceVec);
    } else {
        MR::rotateVecDegree(&vec1, mGravity, 90.0f);
    }

    TVec3f vec2(rVec1);
    vec2.sub(getSensor("body")->mPosition);

    if (0.0f < vec2.dot(vec1)) {
        vec1 = -vec1;
    }
    MR::vecKillElement(vec1, mGravity, &vec1);
    MR::normalizeOrZero(&vec1);
    pOut->set(vec1);
}

void BegomanBase::addVelocityOnPushedFromElectricRail(const TVec3f& rVec1, const TVec3f& rVec2) {
    TVec3f directionAwayFromRail;
    calcDirectionAwayFromRail(&directionAwayFromRail, rVec1, rVec2);

    directionAwayFromRail.setLength(10.0f);
    MR::addVelocityLimit(this, directionAwayFromRail);
    MR::addVelocityLimit(this, mGravity.scaleInline(-20.0f));
}

bool BegomanBase::checkTouchElectricRail(bool notCheckPush) {
    TVec3f vec1;
    TVec3f vec2;
    bool ret = false;

    if (!notCheckPush) {
        if (ElectricRailFunction::isTouchRail(getSensor("pushed_rail"), &vec1, &vec2)) {
            pushedFromElectricRail(getSensor("pushed_rail"), vec1, vec2, 0.5f, 0.5f, true);
            TVec3f sideVec;
            MR::calcSideVec(&sideVec, this);

            if (mInvertSideVec) {
                sideVec.scale(-1.0f);
            }

            if (0.0f < sideVec.dot(vec1.subOperatorInLine(mPosition))) {
                mInvertSideVec = !mInvertSideVec;
            }
        }
    }

    if (ElectricRailFunction::isTouchRail(getSensor("body"), &vec1, &vec2)) {
        ret = onTouchElectric(vec1, vec2);
    }

    if (mIsTouchElectricRail) {
        mIsTouchElectricRail = false;
        mElectricCounter++;
    } else {
        mElectricCounter = 0;
    }
    
    return ret;
}

bool BegomanBase::onTouchElectric(const TVec3f& rVec1, const TVec3f& rVec2) {
    mIsTouchElectricRail = true;

    if (mElectricCounter > 40) {
        MR::emitEffectHit(this, rVec1, "FailureHit");
        addVelocityOnPushedFromElectricRail(rVec1, rVec2);

        mElectricCounter = 0;
        return true;
    } else {
        pushedFromElectricRail(getSensor("body"), rVec1, rVec2, 8.0f, 8.0f, true);
        return false;
    }
}

void BegomanBase::pushedFromElectricRail(HitSensor* pSensor, const TVec3f& rVec1, const TVec3f& rVec2, f32 f1, f32 f2, bool limit) {
    TVec3f directionAwayFromRail;
    calcDirectionAwayFromRail(&directionAwayFromRail, rVec1, rVec2);

    TVec3f vec(pSensor->mPosition);

    vec.sub(rVec1);
    MR::vecKillElement(vec, mGravity, &vec);
    vec.setLength(f2);

    if (limit) {
        MR::addVelocityLimit(this, directionAwayFromRail.scaleInline(f1));
        MR::addVelocityLimit(this, vec);
    } else {
        MR::addVelocity(this, directionAwayFromRail.scaleInline(f1));
        MR::addVelocity(this, vec);
    }
}

void BegomanBase::preventSwingby(f32 f1) {
    if (!MR::isOnGround(this)) {
        f32 mag = mGravity.subOperatorInLine(_A8).length();
        mag *= 0.5f;
        dampingVerticalAndParallelVelocity(f1, mag);
    }
}

void BegomanBase::preventSlopeBlow(f32 f1) {
    if (MR::isOnGround(this)) {
        TVec3f gravityMinusNormal(mGravity);
        gravityMinusNormal.sub(-*MR::getGroundNormal(this));

        f32 mag = gravityMinusNormal.length();
        mag *= 0.5f;
        dampingVerticalAndParallelVelocity(f1, mag);
    }
}

void BegomanBase::dampingVerticalAndParallelVelocity(f32 f1, f32 f2) {
    TVec3f* velocityPointer = &mVelocity;
    TVec3f* gravityPointer = &mGravity;
    // no reason to set these, as the values are overwritten anyway
    TVec3f verticalVelocity(*velocityPointer);
    TVec3f parallelVelocity(*velocityPointer);

    verticalVelocity.scale(gravityPointer->dot(*velocityPointer), *gravityPointer);
    MR::vecKillElement(*velocityPointer, *gravityPointer, &parallelVelocity);

    if (verticalVelocity.dot(*gravityPointer) < 0.0f) {
        verticalVelocity.scale(f2 * f1 + (1 - f2));
    }

    parallelVelocity.scale(f2 * f1 + (1 - f2));
    mVelocity.set(verticalVelocity.addOperatorInLine(parallelVelocity));
}

bool BegomanBase::isInWaterAndSetWaterNerve(const Nerve* pWaterNerve, TPos3f* pPos) {
    // wrong stack offsets and too much stack space;
    WaterInfo info;
    if (MR::getWaterAreaObj(&info, mPosition) != nullptr) {
        MR::getWaterAreaInfo(&info, mPosition, mGravity, false);
        setNerve(pWaterNerve);
        MR::makeMtxUpNoSupportPos(pPos, TVec3f(), TVec3f());
        MR::emitEffect(this, "WaterColumn");
        return true;
    } else {
        return false;
    }
}

void BegomanBase::calcBlowReaction(const TVec3f& rVec1, const TVec3f& rVec2, f32 blowStrengthParallel, f32 blowStrengthVertical) {
    TVec3f blowDirection(rVec2.subOperatorInLine(rVec1));
    TVec3f effectVec(-blowDirection);
    f32 sensorRadius = getSensor("body")->mRadius;

    effectVec.setLength(sensorRadius);
    effectVec.add(getSensor("body")->mPosition);

    MR::emitEffectHit(this, effectVec, "Hit");
    MR::startSound(this, "SE_EM_BEGOMAN_KNOCK_SUCCESS", -1, -1);
    MR::vecKillElement(blowDirection, mGravity, &blowDirection);
    MR::normalizeOrZero(&blowDirection);

    if (MR::isNearZero(blowDirection)) {
        MR::calcFrontVec(&mFaceVec, this);
    } else {
        mFaceVec.set(blowDirection);
    }

    blowDirection.scale(blowStrengthParallel);
    blowDirection.sub(mGravity.scaleInline(blowStrengthVertical));
    mVelocity.set(blowDirection);
}

void BegomanBase::calcAnimCore(TPos3f* pPos) {
    LiveActor::calcAnim();
    pPos->setInline(MR::getJointMtx(this, "Center"));

    TVec3f yDir;
    pPos->getYDirInline(yDir);

    if (!MR::isSameDirection(yDir, mTargetVec, 0.01f)) {
        MR::makeMtxUpFront(pPos, yDir, mTargetVec);
    }
}

bool BegomanBase::calcJointLocator1(TPos3f* pPos, const JointControllerInfo& rJointInfo) {
    TPos3f v9;
    TVec3f zDir;
    // some inline matrix function with _D0 goes here
    pPos->concat(*pPos, v9);
    return true;
}

void BegomanBase::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _C0);

    if (mScaleControler != nullptr) {
        TVec3f stack8;

        stack8.set(mScaleControler->_C);
        _B4.set(stack8);
        stack8.multPS(mScale, stack8);
        MR::setBaseScale(this, stack8);
    }

    mBaseDelegator->registerCallBack();
}

bool BegomanBase::requestAttack() {
    return MR::getSceneObj< BegomanAttackPermitter >(SceneObj_BegomanAttackPermitter)->requestAttack(this);
}

BegomanAttackPermitter::BegomanAttackPermitter(const char* pName) : LiveActor(pName), _8C(nullptr), mBegoman(nullptr), mDistToPlayer(99999.0f), _98(false) {}

void BegomanAttackPermitter::init(const JMapInfoIter& rIter) {
    MR::connectToSceneEnemyDecorationMovement(this);
    MR::invalidateClipping(this);
    makeActorAppeared();
    initNerve(&NrvBegomanAttackPermitter::HostTypeNrvWait::sInstance);
}

void BegomanAttackPermitter::control() {
    _98 = false;
}

bool BegomanAttackPermitter::requestAttack(BegomanBase* pBegoman) {
    if (MR::isValidSwitchA(pBegoman) && !MR::isOnSwitchA(pBegoman)) {
        return false;
    }

    if (pBegoman->getKind() == 0) {
        return !isNerve(&NrvBegomanAttackPermitter::HostTypeNrvPermit::sInstance);
    }

    _98 = true;

    if (isNerve(&NrvBegomanAttackPermitter::HostTypeNrvReceive::sInstance) || isNerve(&NrvBegomanAttackPermitter::HostTypeNrvWait::sInstance)) {
        if (mBegoman == nullptr) {

            mBegoman = pBegoman;
            mDistToPlayer = MR::calcDistanceToPlayer(pBegoman->mPosition);

            if (isNerve(&NrvBegomanAttackPermitter::HostTypeNrvWait::sInstance)) {
                setNerve(&NrvBegomanAttackPermitter::HostTypeNrvReceive::sInstance);
            }
        } else {

            f32 distToPlayerPBegoman = MR::calcDistanceToPlayer(pBegoman->mPosition);
            if (distToPlayerPBegoman < mDistToPlayer) {
                mDistToPlayer = distToPlayerPBegoman;
                mBegoman = pBegoman;
            }
        }
    }

    if (isNerve(&NrvBegomanAttackPermitter::HostTypeNrvPermit::sInstance)) {
        _8C = mBegoman;
        if (mBegoman == pBegoman) {
            // nerve is already HostTypeNrvPermit if code gets here??
            setNerve(&NrvBegomanAttackPermitter::HostTypeNrvPermit::sInstance);
            return true;
        }
    }
    return false;
}

void BegomanAttackPermitter::exeWait() {
    if (MR::isFirstStep(this)) {
        _8C = nullptr;
        mBegoman = nullptr;
        mDistToPlayer = 99999.0f;
    }
}

void BegomanAttackPermitter::exeReceive() {
    if (!_98) {
        setNerve(&NrvBegomanAttackPermitter::HostTypeNrvWait::sInstance);
    } else {
        if (MR::isGreaterStep(this, 2)) {
            setNerve(&NrvBegomanAttackPermitter::HostTypeNrvPermit::sInstance);
        }
    }
}

void BegomanAttackPermitter::exePermit() {
    if (MR::isFirstStep(this)) {
    }

    if (MR::isGreaterStep(this, 30)) {
        setNerve(&NrvBegomanAttackPermitter::HostTypeNrvWait::sInstance);
    }
}
