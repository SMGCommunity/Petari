#include "Game/MapObj/SmallStone.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util.hpp"

namespace {
    const f32 hSize = 100.0f;
    const f32 hBreakSize = 400.0f;
    const f32 hBreakOffsetY = 120.0f;
    const f32 hCircleRadius = 300.0f;
    // hBreakEffectSize
    const f32 hCollisionRadius = 20.0f;
    const f32 hWindRadius = 800.0f;
    const f32 hMaxAnimRate = 4.0f;
    const f32 hMinAnimRate = 0.2f;
    const f32 hCheckLineLength = 1000.0f;
    const f32 hRandomRotateAngle = 360.0f;
    const f32 hRandomScaleMin = 0.75f;
    const f32 hRandomScaleMax = 1.25f;
}  // namespace

void SmallStone_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)3.814697265625e-06f;
    (void)-3.814697265625e-06f;
    (void)0.5f;
    (void)-1.0f;
    (void)1.5707964f;
    (void)2.0f;
    (void)-1.5707964f;
    (void)0.017453292f;
    (void)-360.0f;
    (void)360.0f;
    (void)0.75f;
    (void)1.25f;
    (void)100.0f;
    (void)6.2831855f;
    (void)2607.5945f;
    (void)-2607.5945f;
    (void)300.0f;
    (void)1000.0f;
    (void)0.01f;
    (void)57.29578f;
    (void)120.0f;
    (void)400.0f;
    (void)800.0f;
    (void)0.2f;
    (void)3.8f;
    (void)20.0f;
    (void)40.0f;
    (void)30.0f;
}

SmallStone::SmallStone(const char* pName) : LiveActor(pName), mMembers(), mMemberCount(), mPlacementOffset(), mStoneType(), mUseGravity(true) {
}

SmallStone::~SmallStone() {
    delete[] mMembers;
}

void SmallStone::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjMovement(this);
    MR::initDefaultPos(this, rIter);
    mMemberCount = 8;
    s32 arg = -1;
    MR::getJMapInfoArg0NoInit(rIter, &arg);

    if (arg == -1) {
        mPlacementOffset = 0.0f;
    } else {
        mPlacementOffset = arg;
    }

    arg = -1;
    MR::getJMapInfoArg1NoInit(rIter, &arg);

    if (arg == -1) {
        mUseGravity = true;
    } else {
        mUseGravity = false;
        f32 x = mRotation.x;
        f32 y = mRotation.y;
        f32 z = mRotation.z;
        mRotation.x = x * (PI / 180.0f);
        mRotation.y = y * (PI / 180.0f);
        mRotation.z = z * (PI / 180.0f);
        TQuat4f rotation;
        rotation.setEuler(mRotation.x, mRotation.y, mRotation.z);
        rotation.getYDir(mGravity);
        mGravity.scale(-1.0f);
    }

    arg = -1;
    MR::getJMapInfoArg2NoInit(rIter, &arg);

    if (arg == -1) {
        mIsStarPiece = false;
    } else {
        mIsStarPiece = true;
    }

    const char* pObjectName = nullptr;
    MR::getObjectName(&pObjectName, rIter);

    if (MR::isEqualString(pObjectName, "CircleShell")) {
        mStoneType = 1;
        initMember("CircleShell", mUseGravity);
    } else if (MR::isEqualString(pObjectName, "CircleStrawberry")) {
        mStoneType = 2;
        initMember("CircleStrawberry", mUseGravity);
    } else {
        mStoneType = 0;
        initMember("SmallStone", mUseGravity);
    }

    initSound(4, false);
    appear();
}

void SmallStone::initMember(const char* pModelName, bool useGravity) {
    s32 i;
    mMembers = new SmallStoneMember*[mMemberCount];
    MR::declareStarPiece(this, mMemberCount);

    for (i = 0; i < mMemberCount; i++) {
        SmallStoneMember* pMember = new SmallStoneMember(pModelName);
        pMember->mAnimRate = 1.0f;
        pMember->mShakeRequested = 0;
        pMember->mUseGravity = useGravity;
        pMember->mAnimStopped = false;
        MR::onCalcAnim(pMember);
        MR::invalidateClipping(pMember);
        pMember->appear();

        if (mStoneType == 1) {
            f32 angle = MR::getRandom(-::hRandomRotateAngle, ::hRandomRotateAngle);
            pMember->mRotation.x = 0.0f;
            pMember->mRotation.y = angle;
            pMember->mRotation.z = 0.0f;
            f32 scale = MR::getRandom(::hRandomScaleMin, ::hRandomScaleMax);
            pMember->mScale.set(scale);
            MR::startBva(pMember, "Kind");
            MR::stopBva(pMember);
            s32 max = MR::getBvaFrameMax(pMember, "Kind");
            s32 frame = (max + i) % max;
            MR::setBvaFrameAndStop(pMember, frame);

            switch (frame) {
            case 0:
                pMember->mBreakEffectName = "Break2";
                break;
            case 1:
                pMember->mBreakEffectName = "Break1";
                break;
            case 2:
                pMember->mBreakEffectName = "Break3";
                break;
            case 3:
                pMember->mBreakEffectName = "Break1";
                break;
            case 4:
                pMember->mBreakEffectName = "Break3";
                break;
            }
        } else {
            mRotation.set(0.0f);
            mScale.set(1.0f);
            pMember->mBreakEffectName = "Break";
        }

        mMembers[i] = pMember;
    }

    if (mStoneType == 1 || mStoneType == 2) {
        for (i = 0; i < mMemberCount; i++) {
            s32 index = MR::getRandom(0, i + 1);
            SmallStoneMember* pMember = mMembers[i];
            SmallStoneMember* pOther = mMembers[index];
            f32 frame = MR::getBvaFrame(pMember);
            MR::setBvaFrame(pMember, MR::getBvaFrame(pOther));
            MR::setBvaFrame(pOther, frame);
            const char* pEffect = pMember->mBreakEffectName;
            pMember->mBreakEffectName = pOther->mBreakEffectName;
            pOther->mBreakEffectName = pEffect;
        }
    }

    initHitSensor(1);
    MR::addHitSensorMapObjSimple(this, "Range", 16, ::hSize, TVec3f(gZeroVec));
}

void SmallStone::initAfterPlacement() {
    TVec3f gravity;

    if (mUseGravity) {
        MR::calcGravityVector(this, mPosition, &gravity, nullptr, 0);
    } else {
        gravity.set(mGravity);
    }

    TVec3f axisX;
    axisX.set(1, 0, 0);
    TVec3f axisY;
    axisY.set(0, 1, 0);
    MR::makeAxisCrossPlane(&axisX, &axisY, gravity);
    HitSensor* pSensor = getSensor("Range");
    pSensor->mRadius = 0.0f;
    f32 angleStep = TWO_PI / mMemberCount;
    TVec3f min;
    min.set(0, 0, 0);
    TVec3f max;
    max.set(0, 0, 0);
    s32 i;
    bool failed = false;

    for (i = 0; i < mMemberCount; i++) {
        SmallStoneMember* pMember = mMembers[i];
        TVec3f offset(axisX);
        offset.scale(JMACosRadian(angleStep * i));
        offset.add(axisY * JMASinRadian(angleStep * i));
        offset.scale(::hCircleRadius);
        TVec3f start(mPosition);
        start.add(offset);
        start.sub(gravity);
        failed |= !MR::getFirstPolyOnLineToMap(&pMember->mPosition, nullptr, start, gravity * ::hCheckLineLength);

        if (mUseGravity) {
            MR::calcGravityVector(pMember, &pMember->mGravity, nullptr, 0);
        } else {
            pMember->mGravity.set(gravity);
        }

        TVec3f& rPosition = pMember->mPosition;
        rPosition += -pMember->mGravity * mPlacementOffset;
        TVec3f front;
        front.set(1, 0, 0);

        if (MR::isSameDirection(pMember->mGravity, front, 0.01f)) {
            front.set(0, 1, 0);
        }

        TPos3f posture;
        MR::calcMtxFromGravityAndZAxis(&posture, pMember, pMember->mGravity, front);

        if (mStoneType == 1 || mStoneType == 2) {
            TPos3f rotation;
            MR::makeMtxRotate(rotation, pMember->mRotation);
            posture.concat(rotation);
        }

        TVec3f euler;
        posture.getEuler(euler);
        pMember->mRotation.set(euler.x * _180_PI, euler.y * _180_PI, euler.z * _180_PI);
        pMember->calcAnim();

        TVec3f memberMin(pMember->mPosition);
        TVec3f memberMax(pMember->mPosition);
        f32 radius;
        MR::calcModelBoundingRadius(&radius, pMember);
        memberMin -= TVec3f(radius, radius, radius);
        memberMax += TVec3f(radius, radius, radius);

        if (i == 0) {
            min.set(memberMin);
            max.set(memberMax);
        } else {
            if (memberMin.x < min.x) {
                min.x = memberMin.x;
            }

            if (memberMin.y < min.y) {
                min.y = memberMin.y;
            }

            if (memberMin.z < min.z) {
                min.z = memberMin.z;
            }

            if (max.x < memberMax.x) {
                max.x = memberMax.x;
            }

            if (max.y < memberMax.y) {
                max.y = memberMax.y;
            }

            if (max.z < memberMax.z) {
                max.z = memberMax.z;
            }
        }
    }

    TVec3f center((max + min) / 2.0f);
    mPosition.set(center);
    f32 radius = 0.0f;

    for (s32 i = 0; i < mMemberCount; i++) {
        SmallStoneMember* pMember = mMembers[i];
        TVec3f offset(pMember->mPosition);
        offset.sub(center);
        f32 memberRadius;
        MR::calcModelBoundingRadius(&memberRadius, pMember);
        f32 extent = memberRadius + offset.length();

        if (radius < extent) {
            radius = extent;
        }
    }

    getSensor("Range")->mRadius = radius;
    MR::setClippingTypeSphere(this, 2.0f * radius);
}

void SmallStone::control() {
    for (s32 i = 0; i < mMemberCount; i++) {
        mMembers[i]->movementByHost(this);
    }
}

bool SmallStone::isAllMemberBreak() {
    bool all = true;

    for (s32 i = 0; i < mMemberCount; i++) {
        all &= MR::isHiddenModel(mMembers[i]);
    }

    return all;
}

bool SmallStone::tryBreak() {
    MR::calcGravityVector(this, mPosition, &mGravity, nullptr, 0);
    TVec3f distance(mPosition);
    distance.add(-mGravity * ::hBreakOffsetY);
    distance.sub(*MR::getPlayerPos());
    f32 radius = ::hBreakSize;

    if (distance.squared() > radius * radius) {
        return false;
    }

    for (s32 i = 0; i < mMemberCount; i++) {
        mMembers[i]->mBreakTimer = 0;
    }

    if (isAllMemberBreak()) {
        kill();
    }

    return true;
}

bool SmallStone::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_SPIN_STORM_RANGE) {
        for (s32 i = 0; i < mMemberCount; i++) {
            mMembers[i]->tryShake();
        }

        return true;
    }

    return false;
}

bool SmallStone::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isMsgPlayerSpinAttack(msg)) {
        return false;
    }

    tryBreak();
    return false;
}

void SmallStone::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver) || MR::isSensorEnemy(pReceiver)) {
        for (s32 i = 0; i < mMemberCount; i++) {
            mMembers[i]->tryPush(pSender, pReceiver);
        }
    }
}

void SmallStone::startClipped() {
    LiveActor::startClipped();

    for (s32 i = 0; i < mMemberCount; i++) {
        mMembers[i]->startClipped();
    }
}

void SmallStone::endClipped() {
    LiveActor::endClipped();

    for (s32 i = 0; i < mMemberCount; i++) {
        mMembers[i]->endClipped();
    }
}

SmallStoneMember::SmallStoneMember(const char* pModelName)
    : ModelObj("SmallStoneMember", pModelName, nullptr, MR::DrawBufferType_MapObjStrongLight, MR::MovementType_None, -2, false), mAnimRate(1.0f),
      mShakeRequested(), mAnimStopped(), mBreakTimer(-1), mBreakEffectName(), mUseGravity(true) {
}

bool SmallStoneMember::tryShake() {
    TVec3f distance(*MR::getPlayerPos());
    distance.sub(mPosition);
    MR::vecKillElement(distance, mGravity, &distance);
    TVec3f zero(gZeroVec);
    f32 y = MR::abs(distance.y - zero.y);
    f32 x = MR::abs(distance.x - zero.x);
    f32 z = MR::abs(distance.z - zero.z);
    f32 ratio = (z + (x + y)) / ::hWindRadius;

    if (ratio > 1.0f) {
        ratio = 1.0f;
    }

    f32 intensity = 1.0f - ratio;

    if (intensity == 0.0f) {
        return false;
    }

    f32 rate = ::hMinAnimRate + (::hMaxAnimRate - ::hMinAnimRate) * intensity;
    mShakeRequested = 1;
    MR::setBckRate(this, rate);
    mAnimRate = rate;
    return true;
}

bool SmallStoneMember::tryPush(HitSensor* pSender, HitSensor* pReceiver) {
    TVec3f distance(pReceiver->mPosition);
    distance.sub(mPosition);
    const f32 collisionRadius = ::hCollisionRadius * mScale.y;
    const f32 otherRadius = pReceiver->mRadius;
    f32 radius = collisionRadius + otherRadius;
    radius *= radius;

    if (distance.squared() < radius) {
        f32 oldRadius = pSender->mRadius;
        pSender->mRadius = ::hCollisionRadius * mScale.y;
        TVec3f oldPosition(pSender->mPosition);
        pSender->mPosition.set(mPosition);
        pReceiver->receiveMessage(ACTMES_PUSH, pSender);
        pSender->mPosition.set(oldPosition);
        pSender->mRadius = oldRadius;
        return true;
    }

    return false;
}

void SmallStoneMember::animControl() {
    if (mShakeRequested == 1) {
        if (!MR::isBckPlaying(this, "WindLoop")) {
            mAnimStopped = false;
            MR::onCalcAnim(this);
            MR::startBck(this, "WindLoop");
        }

        mShakeRequested = 0;
    } else {
        if (MR::isBckPlaying(this, "WindLoop")) {
            mAnimStopped = false;
            MR::onCalcAnim(this);
            MR::startBck(this, "WindEnd");
            MR::setBckRate(this, mAnimRate);
        } else if (!mAnimStopped && MR::isBckStopped(this)) {
            mAnimStopped = true;
            MR::offCalcAnim(this);
        }
    }
}

void SmallStoneMember::doBreak(SmallStone* pHost) {
    MR::emitEffect(this, mBreakEffectName);

    if (pHost->mStoneType == 2) {
        MR::startSound(this, "SE_OJ_STRAWBERRY_BREAK");
    } else {
        MR::startSound(this, "SE_OJ_SMALL_STONE_BREAK");
    }

    if (mUseGravity) {
        MR::appearStarPiece(pHost, mPosition, 1, 0.0f, 40.0f, pHost->mIsStarPiece);
    } else {
        TVec3f position = mPosition - mGravity * 30.0f;
        MR::appearStarPiece(pHost, position, 1, 0.0f, 40.0f, pHost->mIsStarPiece);
    }

    MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
    MR::offCalcAnim(this);
    MR::hideModel(this);
}

void SmallStoneMember::movementByHost(SmallStone* pHost) {
    animControl();

    if (mBreakTimer > -1) {
        mBreakTimer--;

        if (mBreakTimer == -1) {
            doBreak(pHost);
        }
    }

    if (MR::isValidCalcAnim(this)) {
        MR::updateModelAnimPlayer(this);
    }
}

SmallStoneMember::~SmallStoneMember() {
}
