#include "Game/Ride/BigBubble.hpp"
#include "Game/AreaObj/BigBubbleGoalArea.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/BigBubbleMoveLimitter.hpp"
#include "Game/MapObj/BigFanHolder.hpp"
#include "Game/MapObj/ElectricRailHolder.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util.hpp"
#include "Game/Util/OctahedronBezierSurface.hpp"
#include <JSystem/JMath/JMATrigonometric.hpp>
#include <revolution/gx/GXTev.h>
#include <revolution/mtx.h>
#include <revolution/types.h>

void BigBubble_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)2.0f;
    (void)3.0f;
}

namespace {
    // static const s32 sBrakeInPointerTime
    // static const f32 sBrakeFric
    static const f32 sNormalFric = 0.992f;
    static const f32 sBuoyancyAccel = 0.03f;
    static const f32 sPushedAccel = 0.18f;
    static const f32 sPushDeformAccel = 1.1f;
    static const f32 sNeedPushMoveMinRate = 0.9f;
    static const f32 sNeedPushDeformMinRate = 0.9f;
    static const f32 sBaseRadiusSphere = 150.0f;
    static const f32 sBaseRadiusCube = 180.0f;
    static const f32 sBaseRadiusOctahedron = 120.0f;
    // static const s32 sNeedPushPointInTime
    // static const f32 sPushPower
    // static const f32 sInPushPower
    // static const s32 sPushInvelidTime
    // static const s32 sUnInvertPushTime
    // static const s32 sPushAccelTime
    // static const f32 sPointScreenAccelMin
    // static const f32 sPointScreenAccelMax
    static const f32 sPointAccelMin = 0.0f;
    static const f32 sPointAccelMax = 1.0f;
    static const f32 sPointAccelDistanceMin = 50.0f;
    static const f32 sPointAccelDistanceMax = 1000.0f;
    static const f32 sPointAccelDeformPowerRate = 1.5f;
    static const f32 sStarPieceDeformPower = 15.0f;
    // static const f32 sMeshScaleK =
    static const f32 sMeshScaleFreq = 0.93f;
    static const f32 sPushMeshPower = 0.02f;
    static const f32 sPressPower = 10.0f;
    static const f32 sAccelDeformPower = 0.8f;
    static const f32 sMaxDeformSpeed = 10.0f;
    static const f32 sSpinStartScreenSpeed = 0.03f;
    static const f32 sSpinMaxScreenSpeed = 0.2f;
    static const f32 sSpinPower = 0.03f;
    static const f32 sSpinFreq = 0.99f;
    static const f32 sMaxSpinSpeed = 0.2f;
    static const f32 sCaptureStartVolume = 0.75f;
    static const f32 sCaptureEndVolume = 0.25f;
    // static const s32 sObstructCaptureTime =
    static const f32 sWindMinDistance = 0.0f;
    static const f32 sWindMaxDistance = 0.5f;
    static const f32 sWindAccel = 0.25f;
    static const f32 sCorioriRotSpeed = 3.0f;
    static const f32 sCoriolisAccelPower = 0.1f;
    static const s32 sReduceSizeInterval = 120;
    static const f32 sReduceVolume = 0.5f;
    static const f32 sMaxVolume = 5.0f;
    // static const f32 sMargeableVolume
    static const s32 sAppearTime = 30;
    static const f32 sAppearAddMoment = 0.03f;
    static const s32 sWarningCycle = 40;
    static const s32 sAutoBreakTime = 1200;
    static const s32 sBreakTime = 20;
    static const s32 sEscapeTime = 30;
    static const f32 sEscapeVelocity = 30.0f;
    static const f32 sMargeableRate = 0.9f;
    static const f32 sMergeDeformAccel = 0.75f;
    static const s32 sMergedTime = 45;
    static const f32 sMergeRotateRate = 0.2f;
    static const f32 sMergeDirCorrection = 0.95f;
    static const s32 sTurchDeformTime = 25;
    static const f32 sTurchDeformAccel = sMergeDeformAccel / 100.0f;
    static const f32 sRadiusSubLevel2 = 3.0f;
    static const f32 sRadiusSubLevel3 = 7.0f;
    static const f32 sRadiusSubLevel4 = 15.0f;
    static const f32 sRadiusSubLevel5 = 30.0f;
    static const f32 sRadiusSubLevel6 = 50.0f;
    static const f32 sRadiusSubLevel1 = 0.0f;
};  // namespace

enum Side { Side_Top = 0, Side_Right = 1, Side_Back = 2, Side_Left = 3, Side_Front = 4, Side_Bottom = 5 };

namespace NrvBigBubble {
    NEW_NERVE(BigBubbleNrvAppear, BigBubble, Appear);
    NEW_NERVE(BigBubbleNrvWait, BigBubble, Wait);
    NEW_NERVE_ONEND(BigBubbleNrvCapture, BigBubble, Capture, Capture);
    NEW_NERVE(BigBubbleNrvBreak, BigBubble, Break);
    NEW_NERVE(BigBubbleNrvEscape, BigBubble, Escape);
    NEW_NERVE(BigBubbleNrvGoal, BigBubble, Goal);
    NEW_NERVE(BigBubbleNrvMerged, BigBubble, Merged);
};  // namespace NrvBigBubble

BigBubble::BigBubble(const char* pName)
    : LiveActor(pName), mSurface(), mMoveLimitter(), mHost(), mRider(), mMergeBubble(), mBubbleQuat(0, 0, 0, 1), mRiderQuat(0, 0, 0, 1),
      mRiderPos(0.0f, 0.0f, 0.0f), mSpawnPosition(0.0f, 0.0f, 0.0f), _18C(0.0f, 0.0f, 0.0f), mPointerPos(0.0f, 0.0f, 0.0f),
      mPushDirection(0.0f, 0.0f, 0.0f), mMoment(0.0f, 0.0f, 0.0f), mCoriolisAccel(1.0f, 0.0f, 0.0f), mInterpolateRate(), mPointAccel(),
      mPushInvalidTime(), mReduceVolumeTimer(), _208(), mMergeIndex(-1), mShapeType(-1), _214(), mAppearRadius(1.0f), mVolume(1.0f), mDrawZ(),
      mBinderRadius(), mWarningColor(255, 0, 0, 0), mIsObstruct(), _22D(), mIsHidden(), mIsExitLimitter(), mIsBroken(), mIsShrinkable(true),
      mIsAttached(true) {
    mSurface = new OctahedronBezierSurface(3);
    mEffectMtx.identity();
    mBaseMtx.identity();

    for (s32 idx = 0; idx < 6; idx++) {
        mDeformCoeff[idx] = 1.0f;
        mDeformSpeed[idx] = 0.0f;
        mMergeBubbles[idx] = nullptr;
    }
}

void BigBubble::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    mBaseMtx.setTrans(mPosition);
    MR::connectToScene(this, MR::MovementType_Ride, MR::CalcAnimType_MapObj, MR::DrawBufferType_None, MR::DrawType_None);
    BigBubble::initSensor();
    initNerve(GET_NERVE(BigBubble, BigBubbleNrvWait));
    initBinder(getBaseRadius(), 0, 16);
    initEffectKeeper(0, "BigBubble", false);
    MR::setEffectHostMtx(this, "PushWind", mEffectMtx);
    initSound(6, false);
    MR::onCalcGravity(this);
    MR::initShadowVolumeSphere(this, getBaseRadius() * 0.8f);
    MR::initStarPointerTarget(this, getBaseRadius(), TVec3f(0.0f, 0.0f, 0.0f));
    if (MR::isValidInfo(rIter))
        makeActorAppeared();
    else
        makeActorDead();
    MR::invalidateClipping(this);
}

void BigBubble::initSensor() {
    initHitSensor(3);
    MR::addHitSensorRide(this, "ride", 16, getBaseRadius(), TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensor(this, "body", ATYPE_BIG_BUBBLE, 16, getBaseRadius(), TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorBinder(this, "bind", 16, getBaseRadius(), TVec3f(0.0f, 0.0f, 0.0f));
}

void BigBubble::makeActorAppeared() {
    updateMeshPoint();
    mSurface->calcAnim();
    LiveActor::makeActorAppeared();
    setNerve(GET_NERVE(BigBubble, BigBubbleNrvAppear));

    for (s32 idx = 0; idx < 6; idx++) {
        mDeformSpeed[idx] = 0.0f;
        mDeformCoeff[idx] = 0.0f;
        mMergeBubbles[idx] = nullptr;
    }

    mMergeIndex = -1;
    mMergeBubble = nullptr;
    mIsExitLimitter = false;
    mIsBroken = false;
}

void BigBubble::makeActorDead() {
    mRider = nullptr;
    mHost = nullptr;
    LiveActor::makeActorDead();

    for (s32 idx = 0; idx < 6; idx++) {
        mDeformCoeff[idx] = 0.0f;
        mDeformSpeed[idx] = 0.0f;
        mMergeBubbles[idx] = nullptr;
    }

    mMergeBubble = nullptr;
}

void BigBubble::control() {
    mIsHidden = MR::isJudgedToClipFrustum(mPosition, getSize() * 1.2f);

    if (!isNerve(GET_NERVE(BigBubble, BigBubbleNrvMerged))) {
        doMoveLimit();
        updatePose();
        if (!mIsHidden) {
            updateMeshPoint();
        }
    }

    if (_208 > 0) {
        _208--;
    }

    if (mPushInvalidTime > 0) {
        mPushInvalidTime--;
    }

    f32 radius = MR::getBinderRadius(this);
    MR::setBinderRadius(this, 0.1f * mBinderRadius + 0.9f * radius);
}

void BigBubble::calcAnim() {
    if (isDraw()) {
        f32 radius = MR::calcPointRadius2D(mPosition, getSize());

        s32 divideLevel = 0;
        if (radius >= ::sRadiusSubLevel6) {
            divideLevel = 6;
        } else if (radius >= ::sRadiusSubLevel5) {
            divideLevel = 5;
        } else if (radius >= ::sRadiusSubLevel4) {
            divideLevel = 4;
        } else if (radius >= ::sRadiusSubLevel3) {
            divideLevel = 3;
        } else if (radius >= ::sRadiusSubLevel2) {
            divideLevel = 2;
        } else if (radius >= ::sRadiusSubLevel1) {
            divideLevel = 1;
        }

        mSurface->setDivideLevel(divideLevel);
        mSurface->calcAnim();
    }

    f32 dot = MR::getCamZdir().dot(mPosition);
    mDrawZ = dot + mScale.x;
}

void BigBubble::draw() const {
    if (isDraw()) {
        GXSetTevColor(GX_TEVREG1, mWarningColor);
        mSurface->draw();
    }
}

void BigBubble::generate(const TVec3f& rPos, const TVec3f& rUp, f32 volume, bool isObstruct, s32 actionType, s32 shapeType,
                         const BigBubbleMoveLimitter* pLimitter) {
    setShapeType(shapeType);
    setActionType(actionType);

    mPosition.set(rPos);
    mBaseMtx.setQT(mBubbleQuat, mPosition);
    mMoveLimitter = pLimitter;
    if (mMoveLimitter != nullptr) {
        mMoveLimitter->limitPosition(&mPosition, getSize());
    }

    mIsObstruct = isObstruct;
    mAppearRadius = getRadius(volume);

    resetDeformVelocity();
    setScale(0.0f);
    MR::setBinderRadius(this, 0.0f);
    mVolume = volume;

    MR::invalidateHitSensors(this);
    updateMeshPoint();
    makeActorAppeared();

    MR::getRandomVector(&mCoriolisAccel, 1.0f);
    mCoriolisAccel.orthogonalize(mGravity);
    MR::onBind(this);
    MR::offCalcGravity(this);
    mGravity.set(-rUp);
    mWarningColor.a = 0;
}

void BigBubble::setActionType(s32 actionType) {
    switch (actionType) {
    case 0:
        mIsShrinkable = false;
        mIsAttached = true;
        break;
    case 1:
        mIsShrinkable = false;
        mIsAttached = false;
        break;
    }
}

void BigBubble::setShapeType(s32 shapeType) {
    mShapeType = shapeType;
    switch (mShapeType) {
    case ShapeType_Sphere:
        mSurface->setDiagonalRate(0.6f);
        break;
    case ShapeType_Cube:
        mSurface->setDiagonalRate(0.9f);
        break;
    case ShapeType_Octahedron:
        mSurface->setDiagonalRate(0.1f);
        break;
    default:
        break;
    }
}

f32 BigBubble::getBaseRadius() const {
    switch (mShapeType) {
    case ShapeType_Sphere:
        return ::sBaseRadiusSphere;
    case ShapeType_Cube:
        return ::sBaseRadiusCube;
    case ShapeType_Octahedron:
        return ::sBaseRadiusOctahedron;
    default:
        return ::sBaseRadiusSphere;
    }
}

void BigBubble::setHost(LiveActor* mActor) {
    mHost = mActor;
}

void BigBubble::setScale(f32 scale) {
    f32 radius = getBaseRadius();
    mScale.set(TVec3f(scale, scale, scale));
    MR::setStarPointerTargetRadius3d(this, radius * scale);
    MR::setShadowVolumeSphereRadius(this, nullptr, 0.8f * radius * scale);
    mBinderRadius = radius * scale * 0.95f;
    MR::setBinderRadius(this, radius * scale * 0.9f);
    MR::setSensorRadius(this, "body", radius * scale);
    MR::setSensorRadius(this, "bind", radius * scale);
    MR::setSensorRadius(this, "ride", radius * scale);
}

void BigBubble::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if ((mRider == nullptr || !MR::tryGetItem(pSender, pReceiver)) && isPushable() && pSender->isType(ATYPE_BIG_BUBBLE) &&
        pReceiver->isType(ATYPE_BIG_BUBBLE)) {
        pReceiver->receiveMessage(ACTMES_PUSH, pSender);
        f32 otherScale = pReceiver->mHost->mScale.x;
        f32 thisScale = mScale.x;
        if (isNerve(GET_NERVE(BigBubble, BigBubbleNrvCapture)) || thisScale >= otherScale) {
            s32 mergeIndex = getMergeIndex(MR::getSensorPos(pReceiver));
            if (mergeIndex != -1 && pReceiver->receiveMessage(ACTMES_BIG_BUBBLE_MERGE, pSender)) {
                mReduceVolumeTimer = 0;
                mMergeBubbles[mergeIndex] = static_cast< BigBubble* >(pReceiver->mHost);
            }
        }
    }
}

bool BigBubble::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceReflect(msg)) {
        TVec3f sensorDir;
        MR::calcSensorDirectionNormalize(&sensorDir, pSender, pReceiver);
        addDeformVelocityOuter(sensorDir * ::sStarPieceDeformPower, false);
        return true;
    }

    if (MR::isMsgPlayerHipDrop(msg) && mRider == nullptr) {
        setNerve(GET_NERVE(BigBubble, BigBubbleNrvBreak));
        return true;
    }

    return false;
}

bool BigBubble::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorRide(pReceiver) || !isEnemyAttackBreakable()) {
        return false;
    }

    if (mRider != nullptr) {
        MR::endBindAndPlayerDamageMsg(this, msg);
        mRider = nullptr;
    }

    setNerve(GET_NERVE(BigBubble, BigBubbleNrvBreak));
    return true;
}

bool BigBubble::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (pSender->isType(ATYPE_BIG_BUBBLE)) {
        if (isNerve(GET_NERVE(BigBubble, BigBubbleNrvWait)) || isNerve(GET_NERVE(BigBubble, BigBubbleNrvCapture))) {
            TVec3f dir = pReceiver->mPosition - pSender->mPosition;
            f32 dist = dir.length();
            f32 sumRadii = pReceiver->getRadius() + pSender->getRadius();
            f32 pushMoveRate = sumRadii * ::sNeedPushMoveMinRate - dist;
            MR::normalizeOrZero(&dir);
            if (MR::isNearZero(dir)) {
                MR::addRandomVector(&dir, dir, 1.0f);
                MR::normalizeOrZero(&dir);
            }

            if (pushMoveRate > 0.0f) {
                TVec3f moveRate = dir * (pushMoveRate / getSize());
                mVelocity.add(moveRate * ::sPushedAccel);
            }

            f32 pushDeformRate = sumRadii * ::sNeedPushDeformMinRate - dist;
            if (pushDeformRate > 0.0f) {
                addDeformVelocityOuter(dir * (pushDeformRate * (::sPushDeformAccel / getBaseRadius())), false);
            }
        }
        return true;
    }

    return false;
}

bool BigBubble::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_AUTORUSH_BEGIN) {
        return requestBind(pSender);
    }

    if (msg == ACTMES_RUSH_CANCEL) {
        return requestCancelBind();
    }

    if (msg == ACTMES_BIG_BUBBLE_MERGE) {
        return requestMerged(pReceiver, pSender);
    }

    if (msg == ACTMES_BIG_BUBBLE_ASSIMILATE) {
        return requestAssimilate(pReceiver, pSender);
    }

    if (msg == ACTMES_UPDATE_BASEMTX) {
        if (mRider != nullptr) {
            updateBindActorMatrix();
            return true;
        }
        return false;
    }

    return false;
}

bool BigBubble::requestBind(HitSensor* pSensor) {
    if (!isNerve(GET_NERVE(BigBubble, BigBubbleNrvWait))) {
        return false;
    }

    f32 scale = pSensor->mHost->mScale.x;
    if (scale * scale * scale < 1.0f) {
        return false;
    }
    mRider = pSensor->mHost;
    mRiderPos = mRider->mPosition;
    mRiderBasePos = mRiderPos;
    mInterpolateRate = 0.0f;

    TPos3f mtx;
    mtx.set(mRider->getBaseMtx());
    mtx.getQuat(mRiderQuat);

    setNerve(GET_NERVE(BigBubble, BigBubbleNrvCapture));
    return true;
}

bool BigBubble::requestMerged(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(BigBubble, BigBubbleNrvWait))) {
        f32 dist = pReceiver->mPosition.distance(pSender->mPosition);
        f32 radius1 = pSender->mRadius;
        f32 radius2 = pReceiver->mRadius;
        mReduceVolumeTimer = 0;
        if (dist < (radius2 + radius1) * ::sMargeableRate) {
            setNerve(GET_NERVE(BigBubble, BigBubbleNrvMerged));
            mMergeBubble = static_cast< BigBubble* >(pReceiver->mHost);
            mMergeIndex = getNearAxisIndex(MR::getSensorPos(pReceiver));
            return true;
        }
    }

    return false;
}

bool BigBubble::requestAssimilate(HitSensor* pSender, HitSensor* pReceiver) {
    f32 scale = pReceiver->mHost->mScale.x;
    TVec3f dir;
    MR::calcSensorDirectionNormalize(&dir, pSender, pReceiver);
    if (MR::isNearZero(dir)) {
        dir.set(mGravity);
    }

    TVec3f pos;
    pos.set(mPosition + dir * getSize());
    TPos3f mtx;
    MR::makeMtxUpNoSupportPos(&mtx, dir, pos);
    MR::emitEffectHit(this, mtx, "Merge");

    mReduceVolumeTimer = 0;
    mVolume += scale * scale * scale;
    if (mVolume > ::sMaxVolume) {
        mVolume = ::sMaxVolume;
    }

    setScale(getRadius(mVolume));
    addDeformVelocityOuter(dir * scale * 8.0f, true);

    for (s32 idx = 0; idx < 6; idx++) {
        if (mMergeBubbles[idx] == pReceiver->mHost) {
            mMergeBubbles[idx] = nullptr;
            break;
        }
    }

    MR::startSound(this, "SE_OJ_BIG_BUBBLE_MERGE");
    return true;
}

bool BigBubble::requestCancelBind() {
    if (mRider != nullptr) {
        mRider = nullptr;
    }
    mWarningColor.a = 0;
    MR::emitEffect(this, "Break");
    MR::startSound(this, "SE_OJ_BIG_BUBBLE_BREAK");
    mIsBroken = true;
    mRiderBasePos = mRiderPos;
    mInterpolateRate = 0.0f;
    kill();
    return true;
}

bool BigBubble::tryAppearEnd() {
    if (MR::isGreaterEqualStep(this, ::sAppearTime)) {
        setNerve(GET_NERVE(BigBubble, BigBubbleNrvWait));
        return true;
    }
    return false;
}

bool BigBubble::tryBreak() {
    bool breakBubble = false;

    if (MR::isPressedMovingWall(this)) {
        breakBubble = true;
    }

    if (MR::isBindedGroundWater(this)) {
        breakBubble = true;
    }

    if (MR::isBindedDamageFire(this)) {
        if (mRider != nullptr) {
            MR::endBindAndPlayerFireDamage(this);
            mRider = nullptr;
        }
        breakBubble = true;
    }

    if (ElectricRailFunction::isTouchRail(getSensor("body"), nullptr, nullptr)) {
        if (mRider != nullptr) {
            MR::endBindAndPlayerElectricDamage(this);
            mRider = nullptr;
        }
        breakBubble = true;
    }

    if (breakBubble || mIsExitLimitter) {
        setNerve(GET_NERVE(BigBubble, BigBubbleNrvBreak));
        return true;
    }

    return false;
}

bool BigBubble::tryAutoBreak() {
    if (!mIsObstruct && MR::isGreaterStep(this, ::sAutoBreakTime) || mIsExitLimitter) {
        if (mRider != nullptr) {
            MR::endBindAndPlayerWeakGravityLimitJump(this, mVelocity);
            mRider = nullptr;
        }
        setNerve(GET_NERVE(BigBubble, BigBubbleNrvBreak));
        return true;
    }
    return false;
}

bool BigBubble::tryBreakEnd() {
    if (MR::isGreaterStep(this, ::sBreakTime)) {
        kill();
        return true;
    }
    return false;
}

bool BigBubble::tryEscape() {
    if (MR::testSubPadTriggerZ(WPAD_CHAN0)) {
        setNerve(GET_NERVE(BigBubble, BigBubbleNrvEscape));
        return true;
    }
    return false;
}

bool BigBubble::tryEscapeEnd() {
    if (MR::isGreaterStep(this, ::sEscapeTime)) {
        if (mRider != nullptr) {
            MR::calcGravity(this);
            MR::endBindAndPlayerJump(this, mGravity * ::sEscapeVelocity, ::sEscapeTime);
            mRider = nullptr;
        }
        MR::emitEffect(this, "Break");
        MR::startSound(this, "SE_OJ_BIG_BUBBLE_BREAK");
        kill();
        return true;
    }
    return false;
}

bool BigBubble::tryMergedCancel() {
    if (MR::isDead(mMergeBubble)) {
        kill();
        return true;
    }
    return false;
}

bool BigBubble::tryMergedEnd() {
    if (MR::isGreaterStep(this, ::sMergedTime)) {
        if (mMergeBubble->receiveMessage(ACTMES_BIG_BUBBLE_ASSIMILATE, getSensor("body"), mMergeBubble->getSensor("body"))) {
            mMergeBubble = nullptr;
            kill();
            return true;
        }
    }
    return false;
}

bool BigBubble::tryGoal() {
    if (MR::checkBigBubbleGoal(this)) {
        if (mRider != nullptr) {
            MR::endBindAndPlayerWeakGravityLimitJump(this, mVelocity);
            mRider = nullptr;
        }
        setNerve(GET_NERVE(BigBubble, BigBubbleNrvGoal));
        return true;
    }

    return false;
}

void BigBubble::exeAppear() {
    if (MR::isFirstStep(this)) {
        mSpawnPosition = mPosition;
        MR::invalidateHitSensors(this);
        MR::getRandomVector(&mMoment, ::sAppearAddMoment);
        MR::startSound(this, "SE_OJ_BIG_BUBBLE_APPEAR");
    }

    f32 scale = MR::calcNerveValue(this, ::sAppearTime, 0.01f, mAppearRadius);
    setScale(scale);

    mPosition.set(mSpawnPosition + mGravity * (-scale * getBaseRadius()));
    if (tryAppearEnd()) {
        MR::onCalcGravity(this);
        MR::validateHitSensors(this);
        MR::invalidateHitSensor(this, "ride");
    }
}

void BigBubble::exeWait() {
    if (MR::isFirstStep(this)) {
        mPushDirection.zero();
    }

    addCoriolisAccel();

    if (mIsObstruct) {
        MR::zeroVelocity(this);
    } else {
        updateNormalVelocity();
    }

    if (tryAutoBreak()) {
        return;
    }
}

void BigBubble::exeCapture() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("SpaceWait", "BigBubbleCapture");
        mPushDirection.zero();
        mReduceVolumeTimer = 0;
        MR::startSound(this, "SE_OJ_BIG_BUBBLE_MARIO_IN");
        mRiderBasePos = mRiderPos;
        MR::validateHitSensor(this, "ride");
    }

    if (mIsAttached) {
        if (addAccelPointing(WPAD_CHAN0)) {
            MR::makeMtxUpNoSupportPos(&mEffectMtx, mPosition - mPointerPos, mPointerPos);
            MR::emitEffect(this, "PushWind");
        } else {
            MR::deleteEffect(this, "PushWind");
        }

        addAccelMoment(WPAD_CHAN0);
    } else {
        addCoriolisAccel();
    }

    MR::requestStarPointerModeBigBubble(this, mPosition);
    MR::requestBigBubbleGuidance();
    updateNormalVelocity();

    mInterpolateRate = MR::calcNerveEaseInOutRate(this, 30);
    mRiderQuat.slerp(mBubbleQuat, ::sSpinPower);

    if (mIsShrinkable) {
        updateCaptureReduceVolume();
        updateCaptureWarningColor();
    }

    if (!tryEscape() && !tryBreak()) {
        if (tryGoal()) {
            return;
        }
    }
}

void BigBubble::exeMerged() {
    if (tryMergedCancel()) {
        MR::onBind(this);
        return;
    }

    if (MR::isFirstStep(this)) {
        MR::offBind(this);
        MR::zeroVelocity(this);
        mMoment.zero();
    }

    TVec3f mergePos, mergeDir;
    mMergeBubble->calcMergePosition(&mergePos, &mergeDir, this);

    f32 mergeSize = getSize();
    if (mMergeBubble->getSize() < mergeSize) {
        mergeSize = mMergeBubble->getSize2();  // FAKEMATCH: using getSize twice here uninlines
    }

    f32 deformAccel = mergeSize * MR::calcNerveRate(this, ::sTurchDeformTime) * (::sTurchDeformAccel);
    addDeformVelocityOuter(mergeDir * deformAccel, false);
    mMergeBubble->addDeformVelocityOuter(mergeDir * -deformAccel * ::sMergeDeformAccel, false);
    addDeformVelocityInternalOressure();
    addDeformVelocityRebound();
    updateDeformVelocity();

    TVec3f localDir;
    calcLocalDirection(&localDir, mMergeIndex);
    TQuat4f rot;
    rot.setRotate(localDir, -mergeDir, ::sMergeRotateRate);
    mBubbleQuat.mult(rot);
    calcLocalDirection(&localDir, mMergeIndex);
    mPosition.set(mergePos - localDir * mDeformCoeff[mMergeIndex] * ::sMergeDirCorrection);
    MR::rotateQuatMoment(&mBubbleQuat, mMoment);

    mBaseMtx.setQT(mBubbleQuat, mPosition);
    updateMeshPoint();
    if (tryMergedCancel() || tryMergedEnd()) {
        MR::onBind(this);
    }
}

void BigBubble::exeBreak() {
    if (MR::isFirstStep(this)) {
        if (mRider != nullptr) {
            MR::endBindAndPlayerWeakGravityLimitJump(this, mVelocity);
            mRider = nullptr;
        }
        mWarningColor.a = 0;
        MR::emitEffect(this, "Break");
        MR::startSound(this, "SE_OJ_BIG_BUBBLE_BREAK");
        mIsBroken = true;
        mRiderBasePos = mRiderPos;
        mInterpolateRate = 0.0f;
    }

    mInterpolateRate = MR::calcNerveRate(this, ::sBreakTime);
    updateNormalVelocity();
    if (tryBreakEnd()) {
        return;
    }
}

void BigBubble::exeEscape() {
    if (MR::isFirstStep(this)) {
        mWarningColor.a = 0;
        MR::startBckPlayer("SwimDive", "BigBubbleEscape");
        MR::startSoundPlayer("SE_PV_HIP_DROP", -1);
        MR::startSoundPlayer("SE_PM_PRE_HIPDROP", -1);
        mRiderBasePos = mRiderPos;
        mInterpolateRate = 0.0f;
    }

    mInterpolateRate = MR::calcNerveRate(this, ::sEscapeTime);
    MR::calcGravity(this);
    updateNormalVelocity();
    TQuat4f rot;
    TVec3f up;
    mRiderQuat.getYDir(up);
    rot.setRotate(up, mGravity, ::sMaxSpinSpeed);
    mRiderQuat.mult(rot);

    if (tryEscapeEnd()) {
        return;
    }
}

void BigBubble::exeGoal() {
    if (MR::isFirstStep(this)) {
        MR::startSystemSE("SE_SY_READ_RIDDLE_S");
    }

    updateNormalVelocity();
    if (tryBreakEnd()) {
        return;
    }
}

void BigBubble::endCapture() {
    MR::invalidateHitSensor(this, "ride");
    mPointAccel = 0.0f;
}

void BigBubble::addDeformVelocityOuter(const TVec3f& rDir, bool reverse) {
    TVec3f side, up, front;
    mBaseMtx.getXDir(side);
    mBaseMtx.getYDir(up);
    mBaseMtx.getZDir(front);

    f32 projSide = rDir.dot(side);
    f32 projUp = rDir.dot(up);
    f32 projFront = rDir.dot(front);

    if (!reverse) {
        if (projSide < 0.0f) {
            mDeformSpeed[Side_Right] += projSide * ::sAccelDeformPower;
        } else {
            mDeformSpeed[Side_Left] -= projSide * ::sAccelDeformPower;
        }

        if (projUp < 0.0f) {
            mDeformSpeed[Side_Top] += projUp * ::sAccelDeformPower;
        } else {
            mDeformSpeed[Side_Bottom] -= projUp * ::sAccelDeformPower;
        }

        if (projFront < 0.0f) {
            mDeformSpeed[Side_Front] += projFront * ::sAccelDeformPower;
        } else {
            mDeformSpeed[Side_Back] -= projFront * ::sAccelDeformPower;
        }
    } else {
        if (projSide > 0.0f) {
            mDeformSpeed[Side_Right] += projSide * ::sAccelDeformPower;
        } else {
            mDeformSpeed[Side_Left] -= projSide * ::sAccelDeformPower;
        }

        if (projUp > 0.0f) {
            mDeformSpeed[Side_Top] += projUp * ::sAccelDeformPower;
        } else {
            mDeformSpeed[Side_Bottom] -= projUp * ::sAccelDeformPower;
        }

        if (projFront > 0.0f) {
            mDeformSpeed[Side_Front] += projFront * ::sAccelDeformPower;
        } else {
            mDeformSpeed[Side_Back] -= projFront * ::sAccelDeformPower;
        }
    }
}

void BigBubble::addDeformVelocityInternalOressure() {
    f32 size = getSize();
    // BUG: pressure should be from opposing faces.
    f32 pressure = (mDeformCoeff[Side_Top] + mDeformCoeff[Side_Left]) * (mDeformCoeff[Side_Back] + mDeformCoeff[Side_Front]) *
                   (mDeformCoeff[Side_Right] + mDeformCoeff[Side_Bottom]) * 0.125f;
    if (pressure < 0.0f) {
        pressure = 0.0f;
    }
    if (size > 0.01f) {
        for (s32 idx = 0; idx < 6; idx++) {
            mDeformSpeed[idx] += (1.0f - pressure / (size * size * size)) * ::sPressPower;
        }
    }
}

void BigBubble::addDeformVelocityRebound() {
    f32 size = getSize();
    for (s32 idx = 0; idx < 6; idx++) {
        mDeformSpeed[idx] += (size - mDeformCoeff[idx]) * ::sPushMeshPower;
    }
}

void BigBubble::addCoriolisAccel() {
    MR::rotateVecDegree(&mCoriolisAccel, mGravity, ::sCorioriRotSpeed);
    mCoriolisAccel.orthogonalize(mGravity);
    if (MR::isNearZero(mCoriolisAccel)) {
        MR::getRandomVector(&mCoriolisAccel, 1.0f);
    }
    MR::normalizeOrZero(&mCoriolisAccel);
    mVelocity.add(mCoriolisAccel * mScale.x * ::sCoriolisAccelPower);
}

void BigBubble::addAccelMoment(s32 padChannel) {
    TVec3f rotateMoment;
    if (MR::calcStarPointerStrokeRotateMoment(&rotateMoment, mPosition, getSize(), padChannel)) {
        mMoment.add(rotateMoment * ::sSpinStartScreenSpeed);
        f32 spinSpeed = mMoment.length();
        if (spinSpeed > ::sSpinMaxScreenSpeed) {
            mMoment *= ::sSpinMaxScreenSpeed / spinSpeed;
        }
    }
}

bool BigBubble::addAccelPointing(s32 padChannel) {
    mPointAccel = 0.0f;
    if (!MR::testCorePadButtonA(padChannel)) {
        return false;
    }

    if (!MR::isStarPointerInScreen(padChannel)) {
        return false;
    }

    if (MR::isStarPointerPointing(this, padChannel, true, "弱")) {
        return false;
    }

    MR::calcStarPointerWorldPointingPos(&mPointerPos, mPosition, padChannel);
    f32 pointerDist = mPointerPos.distance(mPosition);
    if (pointerDist <= ::sPointAccelDistanceMin) {
        return false;
    }

    if (pointerDist >= ::sPointAccelDistanceMax) {
        return false;
    }

    f32 maxDist = ::sPointAccelDistanceMax;
    mPointAccel = 1.0f - (pointerDist - ::sPointAccelDistanceMin) / (maxDist - ::sPointAccelDistanceMin);
    mPointAccel = mPointAccel * (::sPointAccelMax - ::sPointAccelMin) + ::sPointAccelMin;
    MR::startSystemLevelSE("SE_SY_LV_BIG_BUBBLE_WIND", mPointAccel * 100.0f);

    TVec3f accelDir;
    MR::normalizeOrZero(mPosition - mPointerPos, &accelDir);
    MR::addVelocity(this, accelDir * mPointAccel);
    addDeformVelocityOuter(accelDir * ::sPointAccelDeformPowerRate, false);
    return true;
}

void BigBubble::doMoveLimit() {
    if (mMoveLimitter != nullptr) {
        if (mMoveLimitter->limitPosition(&mPosition, getSize())) {
            mIsExitLimitter = true;
        }
        if (mMoveLimitter->limitVelocity(&mVelocity, mPosition, getSize())) {
            mIsExitLimitter = true;
        }
    }
}

void BigBubble::updatePose() {
    MR::rotateQuatMoment(&mBubbleQuat, mMoment);
    mBaseMtx.setQT(mBubbleQuat, mPosition);
    if (mPushInvalidTime > 0) {
        addDeformVelocityOuter(mPushDirection, false);
    }

    addDeformVelocityOuter(-mGravity * ::sBuoyancyAccel * mScale.x, false);
    TVec3f deformVel;
    MR::clampLength(&deformVel, -mVelocity, ::sMaxDeformSpeed);
    addDeformVelocityOuter(deformVel * (1.0f / ::sMaxDeformSpeed), false);
    addDeformVelocityInternalOressure();
    addDeformVelocityRebound();
    updateDeformVelocity();
}

void BigBubble::updateBindActorMatrix() {
    if (mInterpolateRate < 1.0f) {
        mRiderPos.set(mRiderBasePos * (1.0f - mInterpolateRate) + mPosition * mInterpolateRate);

    } else {
        mRiderPos.set(mPosition);
    }
    TPos3f mtx;
    mtx.setQT(mRiderQuat, mRiderPos);
    MR::setBaseTRMtx(mRider, mtx);
}

void BigBubble::updateNormalVelocity() {
    MR::addVelocityToGravity(this, -::sBuoyancyAccel * mScale.x);
    MR::attenuateVelocity(this, ::sNormalFric);
    // TODO: "windDistance" here makes more sense if the function was using the
    // result of the calculation from BigFan, not BigFanFunction.
    // The value returned by calcWindInfo makes more sense as "windForce".
    // Is this a case of these consts not being used here, or is this a deeper issue?
    TVec3f windDir;
    f32 windDistance;
    BigFanFunction::calcWindInfo(&windDir, mPosition, &windDistance);
    windDistance = MR::normalize(windDistance, ::sWindMinDistance, ::sWindMaxDistance);
    mVelocity.add(windDir * ::sWindAccel * windDistance);
    mMoment.mult(::sSpinFreq);
    MR::reboundVelocityFromEachCollision(this, 0.1f, 0.1f, 0.1f, 0.0f);
}

void BigBubble::resetDeformVelocity() {
    for (s32 idx = 0; idx < 6; idx++) {
        mDeformCoeff[idx] = 0;
        mDeformSpeed[idx] = 0;
    }
}

void BigBubble::updateDeformVelocity() {
    for (s32 idx = 0; idx < 6; idx++) {
        mDeformCoeff[idx] += mDeformSpeed[idx];
        mDeformSpeed[idx] *= ::sMeshScaleFreq;
        if (mDeformCoeff[idx] < 0.0f) {
            mDeformCoeff[idx] = 0;
            mDeformSpeed[idx] = 0;
        }
    }
}

void BigBubble::updateMeshPoint() {
    TVec3f side, up, front;
    mBaseMtx.getXDir(side);
    mBaseMtx.getYDir(up);
    mBaseMtx.getZDir(front);

    mSurface->getVertexPtr(Side_Top)->set(mPosition + up * mDeformCoeff[Side_Top]);
    mSurface->getVertexPtr(Side_Right)->set(mPosition + side * mDeformCoeff[Side_Right]);
    mSurface->getVertexPtr(Side_Back)->set(mPosition - front * mDeformCoeff[Side_Back]);
    mSurface->getVertexPtr(Side_Left)->set(mPosition - side * mDeformCoeff[Side_Left]);
    mSurface->getVertexPtr(Side_Front)->set(mPosition + front * mDeformCoeff[Side_Front]);
    mSurface->getVertexPtr(Side_Bottom)->set(mPosition - up * mDeformCoeff[Side_Bottom]);
    mSurface->calcControlPoint();
}

void BigBubble::updateCaptureReduceVolume() {
    if (mReduceVolumeTimer >= ::sReduceSizeInterval * 3) {
        mVolume -= ::sReduceVolume;

        if (::sCaptureEndVolume <= mVolume && mVolume < ::sCaptureStartVolume * 0.98f) {
            mVolume = ::sCaptureStartVolume * 0.98f;
        }

        if (mVolume < 0.0f) {
            mVolume = 0.0f;
        }

        setScale(getRadius(mVolume));
        mReduceVolumeTimer = 0;
        return;
    }

    mReduceVolumeTimer++;
}

s32 BigBubble::getCycle() const {
    if (mReduceVolumeTimer > ::sReduceSizeInterval * 2) {
        return ::sWarningCycle / 4;
    } else if (mReduceVolumeTimer > ::sReduceSizeInterval * 1) {
        return ::sWarningCycle / 2;
    } else {
        f32 f1 = 0.0f;  // FAKEMATCH: possible strip here.
        return ::sWarningCycle / 1;
    }
}

void BigBubble::updateCaptureWarningColor() {
    // FIXME: compiler optimization of warning cycle load and extra stack use
    // https://decomp.me/scratch/SkVS9

    if (mVolume < ::sCaptureStartVolume) {
        s32 warningCycle = getCycle();

        f32 flashPhase = static_cast< f32 >(mReduceVolumeTimer % warningCycle) / static_cast< f32 >(warningCycle - 1);
        f32 color = (MR::cos(TWO_PI * flashPhase) + 1.0f) * 0.5f;

        mWarningColor.a = color * 128.0f;
    } else {
        mWarningColor.a = 0;
    }
}

void BigBubble::calcMergePosition(TVec3f* pPos, TVec3f* pDir, const BigBubble* pBubble) const {
    s32 mergeIndex = -1;
    for (s32 idx = 0; idx < 6; idx++) {
        if (mMergeBubbles[idx] == pBubble) {
            mergeIndex = idx;
            break;
        }
    }

    if (mergeIndex < 0 || mergeIndex >= 6) {
        return;
    }

    pPos->set(mSurface->getVertex(mergeIndex));
    calcLocalDirection(pDir, mergeIndex);
}

void BigBubble::calcLocalDirection(TVec3f* pDir, s32 mergeIndex) const {
    TVec3f dir;
    switch (mergeIndex) {
    case Side_Top:
        mBubbleQuat.getYDir(dir);
        break;
    case Side_Right:
        mBubbleQuat.getXDir(dir);
        break;
    case Side_Back:
        mBubbleQuat.getZDir(dir);
        dir.negate();
        break;
    case Side_Left:
        mBubbleQuat.getXDir(dir);
        dir.negate();
        break;
    case Side_Front:
        mBubbleQuat.getZDir(dir);
        break;
    case Side_Bottom:
        mBubbleQuat.getYDir(dir);
        dir.negate();
        break;
    }
    pDir->set(dir);
}

s32 BigBubble::getNearAxisIndex(const TVec3f& rPos) const {
    TPos3f mtx;
    mtx.set(getBaseMtx());
    mtx.invert(mtx);

    TVec3f axis;
    mtx.mult(rPos, axis);
    s32 index = MR::getMaxAbsElementIndex(axis);

    switch (index) {
    case 0:
        if (axis.x >= 0.0f) {
            return Side_Right;
        } else {
            return Side_Left;
        }
        break;
    case 1:
        if (axis.y >= 0.0f) {
            return Side_Top;
        } else {
            return Side_Bottom;
        }
        break;
    case 2:
        if (axis.z >= 0.0f) {
            return Side_Front;
        } else {
            return Side_Back;
        }
        break;
    default:
        return -1;
        break;
    }
}

s32 BigBubble::getMergeIndex(const TVec3f& rPos) const {
    s32 index = getNearAxisIndex(rPos);
    if (mMergeBubbles[index] == nullptr) {
        return index;
    }
    return -1;
}

bool BigBubble::isPushable() const {
    if (isNerve(GET_NERVE(BigBubble, BigBubbleNrvWait)) || isNerve(GET_NERVE(BigBubble, BigBubbleNrvCapture))) {
        return true;
    }
    return false;
}

bool BigBubble::isBindMario() const {
    return mRider != nullptr;
}

bool BigBubble::isDraw() const {
    return (!mIsHidden && !mIsBroken && !MR::isDead(this));
}

bool BigBubble::isMerged() const {
    return isNerve(GET_NERVE(BigBubble, BigBubbleNrvMerged));
}

bool BigBubble::isEnemyAttackBreakable() const {
    if (isNerve(GET_NERVE(BigBubble, BigBubbleNrvWait)) || isNerve(GET_NERVE(BigBubble, BigBubbleNrvCapture))) {
        return true;
    }
    return false;
}
