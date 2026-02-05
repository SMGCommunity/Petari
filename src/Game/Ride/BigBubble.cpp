#include "Game/Ride/BigBubble.hpp"
#include "Game/AreaObj/BigBubbleGoalArea.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/BigBubbleMoveLimitter.hpp"
#include "Game/MapObj/BigFanHolder.hpp"
#include "Game/MapObj/ElectricRailHolder.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/OctahedronBezierSurface.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>
#include <JSystem/JGeometry/TVec.hpp>
#include <JSystem/JMath/JMATrigonometric.hpp>
#include <revolution/gx/GXTev.h>
#include <revolution/mtx.h>
#include <revolution/types.h>

namespace {
    // sBrakeInPointerTime
    // sBrakeFric
    // sNormalFric
    // sBuoyancyAccel
    // sPushedAccel
    // sPushDeformAccel
    // sNeedPushMoveMinRate
    // sNeedPushDeformMinRate
    static const f32 sBaseRadiusSphere = 150.0f;
    static const f32 sBaseRadiusCube = 180.0f;
    static const f32 sBaseRadiusOctahedron = 120.0f;
    // sNeedPushPointInTime
    // sPushPower
    // sInPushPower
    // sPushInvelidTime
    // sUnInvertPushTime
    // sPushAccelTime
    // sPointScreenAccelMin
    // sPointScreenAccelMax
    // sPointAccelMin
    // sPointAccelMax
    // sPointAccelDistanceMin
    // sPointAccelDistanceMax
    // sPointAccelDeformPowerRate
    static const f32 sStarPieceDeformPower = 15.0f;
    // sMeshScaleK
    // sMeshScaleFreq
    // sPushMeshPower
    // sPressPower
    // sAccelDeformPower
    // sMaxDeformSpeed
    // sSpinStartScreenSpeed
    // sSpinMaxScreenSpeed
    // sSpinPower
    // sSpinFreq
    // sMaxSpinSpeed
    // sCaptureStartVolume
    // sCaptureEndVolume
    // sObstructCaptureTime
    // sWindMinDistance
    // sWindMaxDistance
    // sWindAccel
    // sCorioriRotSpeed
    // sCoriolisAccelPower
    static const s32 sReduceSizeInterval = 360;
    static const f32 sReduceVolume = 0.5f;
    // sMaxVolume
    // sMargeableVolume
    // sAppearTime
    // sAppearAddMoment
    // sWarningCycle
    static const s32 sAutoBreakTime = 1200;
    // sBreakTime
    // sEscapeTime
    // sEscapeVelocity
    // sMargeableRate
    // sMergeDeformAccel
    // sMergedTime
    // sMergeRotateRate
    // sMergeDirCorrection
    // sTurchDeformTime
    // sTurchDeformAccel
    static const f32 sRadiusSubLevel2 = 5.0f;
    static const f32 sRadiusSubLevel3 = 7.0f;
    static const f32 sRadiusSubLevel4 = 15.0f;
    static const f32 sRadiusSubLevel5 = 30.0f;
    static const f32 sRadiusSubLevel6 = 50.0f;
}  // namespace

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
    : LiveActor(pName), mSurface(nullptr), mMoveLimitter(nullptr), mHost(nullptr), mRider(nullptr), mMergeBubble(nullptr), mBubbleQuat(0, 0, 0, 1),
      mRiderQuat(0, 0, 0, 1), mRiderPos(0.0f, 0.0f, 0.0f), mSpawnPosition(0.0f, 0.0f, 0.0f), _18C(0.0f, 0.0f, 0.0f), mPointerPos(0.0f, 0.0f, 0.0f),
      _1A4(0.0f, 0.0f, 0.0f), mMoment(0.0f, 0.0f, 0.0f), mCoriolisAccel(1.0f, 0.0f, 0.0f), mInterpolateTime(0), mBlowForce(0), _200(0),
      mReduceVolumeTimer(0), _208(0), mMergeIndex(-1), mShapeType(-1), _214(0), mAppearRadius(1.0f), mVolume(1.0f), _220(0.0f), mBinderRadius(0.0f),
      mWarningColor(255, 0, 0, 0), _22C(false), _22D(false), mIsHidden(false), _22F(false), mIsBroken(false), mIsShrinkable(true), mIsAttached(true) {
    mSurface = new OctahedronBezierSurface(3);
    mEffectMtx.identity();
    mBaseMtx.identity();

    for (s32 idx = 0; idx < 6; idx++) {
        mDeformCoeff[idx] = 1.0f;
        mDeformVelocity[idx] = 0.0f;
        mMergeBubbles[idx] = nullptr;
    }
}

void BigBubble::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    mBaseMtx.setTrans(mPosition);
    MR::connectToScene(this, MR::MovementType_Ride, MR::CalcAnimType_MapObj, -1, -1);
    BigBubble::initSensor();
    initNerve(&NrvBigBubble::BigBubbleNrvWait::sInstance);
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
    setNerve(&NrvBigBubble::BigBubbleNrvAppear::sInstance);

    for (s32 idx = 0; idx < 6; idx++) {
        mDeformVelocity[idx] = 0.0f;
        mDeformCoeff[idx] = 0.0f;
        mMergeBubbles[idx] = nullptr;
    }

    mMergeIndex = -1;
    mMergeBubble = nullptr;
    _22F = false;
    mIsBroken = false;
}

void BigBubble::makeActorDead() {
    mRider = nullptr;
    mHost = nullptr;
    LiveActor::makeActorDead();

    for (s32 idx = 0; idx < 6; idx++) {
        mDeformCoeff[idx] = 0.0f;
        mDeformVelocity[idx] = 0.0f;
        mMergeBubbles[idx] = nullptr;
    }

    mMergeBubble = nullptr;
}

void BigBubble::control() {
    // FIXME: float regswaps in setBinderRadius line
    // https://decomp.me/scratch/uE4Rx

    mIsHidden = MR::isJudgedToClipFrustum(mPosition, getSize() * 1.2f);

    if (!isNerve(&NrvBigBubble::BigBubbleNrvMerged::sInstance)) {
        doMoveLimit();
        updatePose();
        if (!mIsHidden) {
            updateMeshPoint();
        }
    }

    if (_208 > 0) {
        _208--;
    }

    if (_200 > 0) {
        _200--;
    }

    MR::setBinderRadius(this, MR::getBinderRadius(this) * 0.9f + mBinderRadius * 0.1f);
}

void BigBubble::calcAnim() {
    if (isDraw()) {
        f32 radius = MR::calcPointRadius2D(mPosition, getSize());

        s32 divideLevel = 0;
        if (radius >= sRadiusSubLevel6) {
            divideLevel = 6;
        } else if (radius >= sRadiusSubLevel5) {
            divideLevel = 5;
        } else if (radius >= sRadiusSubLevel4) {
            divideLevel = 4;
        } else if (radius >= sRadiusSubLevel3) {
            divideLevel = 3;
        } else if (radius >= sRadiusSubLevel2) {
            divideLevel = 2;
        } else if (radius >= 3.0f) {  // no static symbol here?
            divideLevel = 1;
        }

        mSurface->setDivideLevel(divideLevel);
        mSurface->calcAnim();
    }

    f32 dot = MR::getCamZdir().dot(mPosition);
    _220 = dot + mScale.x;
}

void BigBubble::draw() const {
    if (isDraw()) {
        GXSetTevColor(GX_TEVREG1, mWarningColor);
        mSurface->draw();
    }
}

void BigBubble::generate(const TVec3f& rPos, const TVec3f& rUp, f32 volume, bool b, s32 actionType, s32 shapeType,
                         const BigBubbleMoveLimitter* pLimitter) {
    setShapeType(shapeType);
    setActionType(actionType);

    mPosition.set(rPos);
    mBaseMtx.setQT(mBubbleQuat, mPosition);
    mMoveLimitter = pLimitter;
    if (mMoveLimitter != nullptr) {
        mMoveLimitter->limitPosition(&mPosition, getSize());
    }

    _22C = b;
    mAppearRadius = getRadius(volume);

    resetDeformVelocity();
    setScale(0.0f);
    MR::setBinderRadius(this, 0.0f);
    mVolume = volume;

    MR::invalidateHitSensors(this);
    updateMeshPoint();
    makeActorAppeared();

    MR::getRandomVector(&mCoriolisAccel, 1.0f);
    mCoriolisAccel.rejection(mGravity);
    MR::onBind(this);
    MR::offCalcGravity(this);
    mGravity.set(rUp.negateOperatorInternal());
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
        return sBaseRadiusSphere;
    case ShapeType_Cube:
        return sBaseRadiusCube;
    case ShapeType_Octahedron:
        return sBaseRadiusOctahedron;
    default:
        return sBaseRadiusSphere;
    }
}

void BigBubble::setHost(LiveActor* mActor) {
    mHost = mActor;
}

void BigBubble::setScale(f32 scale) {
    f32 radius = getBaseRadius();
    mScale.set(TVec3f(scale, scale, scale));
    MR::setStarPointerTargetRadius3d(this, radius * scale);
    MR::setShadowVolumeSphereRadius(this, nullptr, radius * 0.8f * scale);
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
        if (isNerve(&NrvBigBubble::BigBubbleNrvCapture::sInstance) || thisScale >= otherScale) {
            s32 mergeIndex = getMergeIndex(MR::getSensorPos(pReceiver));
            if (mergeIndex != -1 && pReceiver->receiveMessage(ACTMES_BIG_BUBBLE_MERGE, pSender)) {
                mReduceVolumeTimer = 0;
                mMergeBubbles[mergeIndex] = reinterpret_cast< BigBubble* >(pReceiver->mHost);
            }
        }
    }
}

bool BigBubble::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceReflect(msg)) {
        TVec3f sensorDir;
        MR::calcSensorDirectionNormalize(&sensorDir, pSender, pReceiver);
        addDeformVelocityOuter(sensorDir.multiplyOperatorInline(sStarPieceDeformPower), false);
        return true;
    }

    if (MR::isMsgPlayerHipDrop(msg) && mRider == nullptr) {
        setNerve(&NrvBigBubble::BigBubbleNrvBreak::sInstance);
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

    setNerve(&NrvBigBubble::BigBubbleNrvBreak::sInstance);
    return true;
}

bool BigBubble::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    // FIXME: regswaps and stack order
    // https://decomp.me/scratch/x5Tjr

    if (pSender->isType(ATYPE_BIG_BUBBLE)) {
        if (isNerve(&NrvBigBubble::BigBubbleNrvWait::sInstance) || isNerve(&NrvBigBubble::BigBubbleNrvCapture::sInstance)) {
            TVec3f dir = pReceiver->mPosition - pSender->mPosition;
            f32 dist = dir.length();
            f32 sumRadii = pSender->mRadius + pReceiver->mRadius;
            f32 mergeDiff = sumRadii * 0.9f - dist;
            MR::normalizeOrZero(&dir);
            if (MR::isNearZero(dir)) {
                MR::addRandomVector(&dir, dir, 1.0f);
                MR::normalizeOrZero(&dir);
            }

            if (mergeDiff > 0.0f) {
                mVelocity.add(dir.multiplyOperatorInline(mergeDiff / getSize()).multiplyOperatorInline(0.18f));
            }

            f32 mergeDiff2 = sumRadii * 0.9f - dist;
            if (mergeDiff2 > 0.0f) {
                addDeformVelocityOuter(dir.multiplyOperatorInline(mergeDiff2 * (1.1f / getBaseRadius())), false);
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
    if (!isNerve(&NrvBigBubble::BigBubbleNrvWait::sInstance)) {
        return false;
    }

    f32 scale = pSensor->mHost->mScale.x;
    if (scale * scale * scale < 1.0f) {
        return false;
    }
    mRider = pSensor->mHost;
    mRiderPos = mRider->mPosition;
    mRiderBasePos = mRiderPos;
    mInterpolateTime = 0.0f;

    TPos3f mtx;
    mtx.set(mRider->getBaseMtx());
    mtx.getQuat(mRiderQuat);

    setNerve(&NrvBigBubble::BigBubbleNrvCapture::sInstance);
    return true;
}

bool BigBubble::requestMerged(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvBigBubble::BigBubbleNrvWait::sInstance)) {
        f32 dist = pReceiver->mPosition.distance(pSender->mPosition);
        f32 radius1 = pSender->mRadius;
        f32 radius2 = pReceiver->mRadius;
        mReduceVolumeTimer = 0;
        if (dist < (radius2 + radius1) * 0.9f) {
            setNerve(&NrvBigBubble::BigBubbleNrvMerged::sInstance);
            mMergeBubble = reinterpret_cast< BigBubble* >(pReceiver->mHost);
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
    pos.set(mPosition.addOperatorInLine(dir.multiplyOperatorInline(getSize())));
    TPos3f mtx;
    MR::makeMtxUpNoSupportPos(&mtx, dir, pos);
    MR::emitEffectHit(this, mtx, "Merge");

    mReduceVolumeTimer = 0;
    mVolume += scale * scale * scale;
    if (mVolume > 5.0f) {
        mVolume = 5.0f;
    }

    setScale(getRadius(mVolume));
    addDeformVelocityOuter(dir.multiplyOperatorInline(scale).multiplyOperatorInline(8.0f), true);

    for (s32 idx = 0; idx < 6; idx++) {
        if (mMergeBubbles[idx] == pReceiver->mHost) {
            mMergeBubbles[idx] = nullptr;
            break;
        }
    }

    MR::startSound(this, "SE_OJ_BIG_BUBBLE_MERGE", -1, -1);
    return true;
}

bool BigBubble::requestCancelBind() {
    if (mRider != nullptr) {
        mRider = nullptr;
    }
    mWarningColor.a = 0;
    MR::emitEffect(this, "Break");
    MR::startSound(this, "SE_OJ_BIG_BUBBLE_BREAK", -1, -1);
    mIsBroken = true;
    mRiderBasePos = mRiderPos;
    mInterpolateTime = 0.0f;
    kill();
    return true;
}

bool BigBubble::tryAppearEnd() {
    if (MR::isGreaterEqualStep(this, 30)) {
        setNerve(&NrvBigBubble::BigBubbleNrvWait::sInstance);
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

    if (breakBubble || _22F) {
        setNerve(&NrvBigBubble::BigBubbleNrvBreak::sInstance);
        return true;
    }

    return false;
}

bool BigBubble::tryAutoBreak() {
    if (!_22C && MR::isGreaterStep(this, sAutoBreakTime) || _22F) {
        if (mRider != nullptr) {
            MR::endBindAndPlayerWeakGravityLimitJump(this, mVelocity);
            mRider = nullptr;
        }
        setNerve(&NrvBigBubble::BigBubbleNrvBreak::sInstance);
        return true;
    }
    return false;
}

bool BigBubble::tryBreakEnd() {
    if (MR::isGreaterStep(this, 20)) {
        kill();
        return true;
    }
    return false;
}

bool BigBubble::tryEscape() {
    if (MR::testSubPadTriggerZ(WPAD_CHAN0)) {
        setNerve(&NrvBigBubble::BigBubbleNrvEscape::sInstance);
        return true;
    }
    return false;
}

bool BigBubble::tryEscapeEnd() {
    if (MR::isGreaterStep(this, 30)) {
        if (mRider != nullptr) {
            MR::calcGravity(this);
            MR::endBindAndPlayerJump(this, mGravity.multiplyOperatorInline(30.0f), 30);
            mRider = nullptr;
        }
        MR::emitEffect(this, "Break");
        MR::startSound(this, "SE_OJ_BIG_BUBBLE_BREAK", -1, -1);
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
    if (MR::isGreaterStep(this, 45)) {
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
        setNerve(&NrvBigBubble::BigBubbleNrvGoal::sInstance);
        return true;
    }

    return false;
}

void BigBubble::exeAppear() {
    if (MR::isFirstStep(this)) {
        mSpawnPosition = mPosition;
        MR::invalidateHitSensors(this);
        MR::getRandomVector(&mMoment, 0.03f);
        MR::startSound(this, "SE_OJ_BIG_BUBBLE_APPEAR", -1, -1);
    }

    f32 scale = MR::calcNerveValue(this, 30, 0.01f, mAppearRadius);
    setScale(scale);

    mPosition.set(mSpawnPosition.addOperatorInLine(mGravity.multiplyOperatorInline(-scale * getBaseRadius())));
    if (tryAppearEnd()) {
        MR::onCalcGravity(this);
        MR::validateHitSensors(this);
        MR::invalidateHitSensor(this, "ride");
    }
}

void BigBubble::exeWait() {
    if (MR::isFirstStep(this)) {
        _1A4.zero();
    }

    addCoriolisAccel();

    if (_22C) {
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
        _1A4.zero();
        mReduceVolumeTimer = 0;
        MR::startSound(this, "SE_OJ_BIG_BUBBLE_MARIO_IN", -1, -1);
        mRiderBasePos = mRiderPos;
        MR::validateHitSensor(this, "ride");
    }

    if (mIsAttached) {
        if (addAccelPointing(WPAD_CHAN0)) {
            TVec3f dir(mPosition - mPointerPos);
            MR::makeMtxUpNoSupportPos(&mEffectMtx, dir, mPointerPos);
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

    mInterpolateTime = MR::calcNerveEaseInOutRate(this, 30);
    mRiderQuat.slerp(mBubbleQuat, 0.03f);

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
    // FIXME: reg and instruction swap
    // https://decomp.me/scratch/Wf2Wd

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

    f32 size = getSize();
    if (mMergeBubble->getBaseRadius() * mMergeBubble->mScale.x < size) {
        size = mMergeBubble->getBaseRadius() * mMergeBubble->mScale.x;
    }

    f32 f1 = size * MR::calcNerveRate(this, 25) * 0.0075f;
    addDeformVelocityOuter(mergeDir.multiplyOperatorInline(f1), false);
    mMergeBubble->addDeformVelocityOuter(mergeDir.multiplyOperatorInline(-f1).multiplyOperatorInline(0.75f), false);
    addDeformVelocityInternalOressure();
    addDeformVelocityRebound();
    updateDeformVelocity();

    TVec3f localDir;
    calcLocalDirection(&localDir, mMergeIndex);
    TQuat4f q;
    q.setRotate(localDir, mergeDir.negateOperatorInternal(), 0.2f);
    PSQUATMultiply(&q, &mBubbleQuat, &mBubbleQuat);
    calcLocalDirection(&localDir, mMergeIndex);
    mPosition.set(mergePos - localDir.multiplyOperatorInline(mDeformCoeff[mMergeIndex]).multiplyOperatorInline(0.95f));
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
        MR::startSound(this, "SE_OJ_BIG_BUBBLE_BREAK", -1, -1);
        mIsBroken = true;
        mRiderBasePos = mRiderPos;
        mInterpolateTime = 0.0f;
    }

    mInterpolateTime = MR::calcNerveRate(this, 20);
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
        mInterpolateTime = 0.0f;
    }

    mInterpolateTime = MR::calcNerveRate(this, 30);
    MR::calcGravity(this);
    updateNormalVelocity();
    TQuat4f q;
    TVec3f up;
    mRiderQuat.getYDir(up);

    q.setRotate(up, mGravity, 0.2f);
    PSQUATMultiply(&q, &mRiderQuat, &mRiderQuat);

    if (tryEscapeEnd()) {
        return;
    }
}

void BigBubble::exeGoal() {
    if (MR::isFirstStep(this)) {
        MR::startSystemSE("SE_SY_READ_RIDDLE_S", -1, -1);
    }

    updateNormalVelocity();
    if (tryBreakEnd()) {
        return;
    }
}

void BigBubble::endCapture() {
    MR::invalidateHitSensor(this, "ride");
    mBlowForce = 0.0f;
}

void BigBubble::addDeformVelocityOuter(const TVec3f& rDir, bool b) {
    TVec3f side, up, front;
    mBaseMtx.getXDir(side);
    mBaseMtx.getYDir(up);
    mBaseMtx.getZDir(front);

    f32 projSide = rDir.dot(side);
    f32 projUp = rDir.dot(up);
    f32 projFront = rDir.dot(front);

    if (!b) {
        if (projSide < 0.0f) {
            mDeformVelocity[Side_Right] += projSide * 0.8f;
        } else {
            mDeformVelocity[Side_Left] -= projSide * 0.8f;
        }

        if (projUp < 0.0f) {
            mDeformVelocity[Side_Top] += projUp * 0.8f;
        } else {
            mDeformVelocity[Side_Bottom] -= projUp * 0.8f;
        }

        if (projFront < 0.0f) {
            mDeformVelocity[Side_Front] += projFront * 0.8f;
        } else {
            mDeformVelocity[Side_Back] -= projFront * 0.8f;
        }
    } else {
        if (projSide > 0.0f) {
            mDeformVelocity[Side_Right] += projSide * 0.8f;
        } else {
            mDeformVelocity[Side_Left] -= projSide * 0.8f;
        }

        if (projUp > 0.0f) {
            mDeformVelocity[Side_Top] += projUp * 0.8f;
        } else {
            mDeformVelocity[Side_Bottom] -= projUp * 0.8f;
        }

        if (projFront > 0.0f) {
            mDeformVelocity[Side_Front] += projFront * 0.8f;
        } else {
            mDeformVelocity[Side_Back] -= projFront * 0.8f;
        }
    }
}

void BigBubble::addDeformVelocityInternalOressure() {
    f32 size = getSize();
    // looks like there's a typo here, this is a bug
    f32 pressure = (mDeformCoeff[Side_Top] + mDeformCoeff[Side_Left]) * (mDeformCoeff[Side_Back] + mDeformCoeff[Side_Front]) *
                   (mDeformCoeff[Side_Right] + mDeformCoeff[Side_Bottom]) * 0.125f;
    if (pressure < 0.0f) {
        pressure = 0.0f;
    }
    if (size > 0.01f) {
        for (s32 idx = 0; idx < 6; idx++) {
            mDeformVelocity[idx] += (1.0f - pressure / (size * size * size)) * 10.0f;
        }
    }
}

void BigBubble::addDeformVelocityRebound() {
    f32 radius = getBaseRadius();
    f32 scale = mScale.x;
    for (s32 idx = 0; idx < 6; idx++) {
        mDeformVelocity[idx] += ((radius * scale) - mDeformCoeff[idx]) * 0.02f;
    }
}

void BigBubble::addCoriolisAccel() {
    MR::rotateVecDegree(&mCoriolisAccel, mGravity, 3.0f);
    mCoriolisAccel.rejection(mGravity);
    if (MR::isNearZero(mCoriolisAccel)) {
        MR::getRandomVector(&mCoriolisAccel, 1.0f);
    }
    MR::normalizeOrZero(&mCoriolisAccel);
    mVelocity.add(mCoriolisAccel.multiplyOperatorInline(mScale.x).multiplyOperatorInline(0.1f));
}

void BigBubble::addAccelMoment(s32 padChannel) {
    TVec3f rotateMoment;
    if (MR::calcStarPointerStrokeRotateMoment(&rotateMoment, mPosition, getSize(), padChannel)) {
        mMoment.add(rotateMoment.multiplyOperatorInline(0.03f));
        f32 inertia = mMoment.length();
        if (inertia > 0.2f) {
            mMoment *= 0.2f / inertia;
        }
    }
}

bool BigBubble::addAccelPointing(s32 padChannel) {
    mBlowForce = 0.0f;
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
    if (pointerDist <= 50.0f) {
        return false;
    }

    if (pointerDist >= 1000.0f) {
        return false;
    }

    f32 maxDist = 1000.0f;
    mBlowForce = 1.0f - (pointerDist - 50.0f) / (maxDist - 50.0f);
    MR::startSystemLevelSE("SE_SY_LV_BIG_BUBBLE_WIND", mBlowForce * 100.0f, -1);

    TVec3f accelDir;
    MR::normalizeOrZero(mPosition - mPointerPos, &accelDir);
    MR::addVelocity(this, accelDir.multiplyOperatorInline(mBlowForce));
    addDeformVelocityOuter(accelDir.multiplyOperatorInline(1.5f), false);
    return true;
}

void BigBubble::doMoveLimit() {
    if (mMoveLimitter != nullptr) {
        if (mMoveLimitter->limitPosition(&mPosition, getSize())) {
            _22F = true;
        }
        if (mMoveLimitter->limitVelocity(&mVelocity, mPosition, getSize())) {
            _22F = true;
        }
    }
}

void BigBubble::updatePose() {
    MR::rotateQuatMoment(&mBubbleQuat, mMoment);
    mBaseMtx.setQT(mBubbleQuat, mPosition);
    if (_200 > 0) {
        addDeformVelocityOuter(_1A4, false);
    }

    addDeformVelocityOuter(mGravity.negateOperatorInternal().multiplyOperatorInline(0.03f).multiplyOperatorInline(mScale.x), false);
    TVec3f v1;
    MR::clampLength(&v1, mVelocity.negateOperatorInternal(), 10.0f);
    addDeformVelocityOuter(v1.multiplyOperatorInline(0.1f), false);
    addDeformVelocityInternalOressure();
    addDeformVelocityRebound();
    updateDeformVelocity();
}

void BigBubble::updateBindActorMatrix() {
    if (mInterpolateTime < 1.0f) {
        mRiderPos.set(
            mRiderBasePos.multiplyOperatorInline(1.0f - mInterpolateTime).addOperatorInLine(mPosition.multiplyOperatorInline(mInterpolateTime)));

    } else {
        mRiderPos.set(mPosition);
    }
    TPos3f mtx;
    mtx.setQT(mRiderQuat, mRiderPos);
    MR::setBaseTRMtx(mRider, mtx);
}

void BigBubble::updateNormalVelocity() {
    MR::addVelocityToGravity(this, mScale.x * -0.03f);
    MR::attenuateVelocity(this, 0.992f);
    TVec3f windDir;
    f32 windSpeed;
    BigFanFunction::calcWindInfo(&windDir, mPosition, &windSpeed);
    windSpeed = MR::normalize(windSpeed, 0.0f, 0.5f);
    mVelocity.add(windDir.multiplyOperatorInline(0.25f).multiplyOperatorInline(windSpeed));
    mMoment.mult(0.99f);
    MR::reboundVelocityFromEachCollision(this, 0.1f, 0.1f, 0.1f, 0.0f);
}

void BigBubble::resetDeformVelocity() {
    for (s32 idx = 0; idx < 6; idx++) {
        mDeformCoeff[idx] = 0;
        mDeformVelocity[idx] = 0;
    }
}

void BigBubble::updateDeformVelocity() {
    for (s32 idx = 0; idx < 6; idx++) {
        mDeformCoeff[idx] += mDeformVelocity[idx];
        mDeformVelocity[idx] *= 0.93f;
        if (mDeformCoeff[idx] < 0.0f) {
            mDeformCoeff[idx] = 0;
            mDeformVelocity[idx] = 0;
        }
    }
}

void BigBubble::updateMeshPoint() {
    TVec3f side, up, front;
    mBaseMtx.getXDir(side);
    mBaseMtx.getYDir(up);
    mBaseMtx.getZDir(front);

    mSurface->getVertexPtr(Side_Top)->set(mPosition.addOperatorInLine(up.multiplyOperatorInline(mDeformCoeff[Side_Top])));
    mSurface->getVertexPtr(Side_Right)->set(mPosition.addOperatorInLine(side.multiplyOperatorInline(mDeformCoeff[Side_Right])));
    mSurface->getVertexPtr(Side_Back)->set(mPosition - front.multiplyOperatorInline(mDeformCoeff[Side_Back]));
    mSurface->getVertexPtr(Side_Left)->set(mPosition - side.multiplyOperatorInline(mDeformCoeff[Side_Left]));
    mSurface->getVertexPtr(Side_Front)->set(mPosition.addOperatorInLine(front.multiplyOperatorInline(mDeformCoeff[Side_Front])));
    mSurface->getVertexPtr(Side_Bottom)->set(mPosition - up.multiplyOperatorInline(mDeformCoeff[Side_Bottom]));
    mSurface->calcControlPoint();
}

void BigBubble::updateCaptureReduceVolume() {
    if (mReduceVolumeTimer >= sReduceSizeInterval) {
        mVolume -= sReduceVolume;

        if (0.25f <= mVolume && mVolume < 0.735f) {
            mVolume = 0.735f;
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

void BigBubble::updateCaptureWarningColor() {
    // FIXME: horrible inlining and compiler optimization nightmare
    // https://decomp.me/scratch/teRSV

    if (mVolume < 0.75f) {
        // bad
        s32 flashTime;
        if (mReduceVolumeTimer > 240) {
            flashTime = 10;
        } else if (mReduceVolumeTimer > 120) {
            flashTime = 20;
        } else {
            flashTime = 40;
        }

        // disgusting mess
        f32 flashPhase = (static_cast< f32 >(mReduceVolumeTimer % flashTime) / static_cast< f32 >(flashTime - 1)) * TWO_PI;

        mWarningColor.a = (JMath::sSinCosTable.cosLapRad(flashPhase) + 1.0f) * 0.5f * 128.0f;
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
        dir.negateInternal();
        break;
    case Side_Left:
        mBubbleQuat.getXDir(dir);
        dir.negateInternal();
        break;
    case Side_Front:
        mBubbleQuat.getZDir(dir);
        break;
    case Side_Bottom:
        mBubbleQuat.getYDir(dir);
        dir.negateInternal();
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
    if (isNerve(&NrvBigBubble::BigBubbleNrvWait::sInstance) || isNerve(&NrvBigBubble::BigBubbleNrvCapture::sInstance)) {
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
    return isNerve(&NrvBigBubble::BigBubbleNrvMerged::sInstance);
}

bool BigBubble::isEnemyAttackBreakable() const {
    if (isNerve(&NrvBigBubble::BigBubbleNrvWait::sInstance) || isNerve(&NrvBigBubble::BigBubbleNrvCapture::sInstance)) {
        return true;
    }
    return false;
}
