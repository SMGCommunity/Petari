#include <revolution/types.h>

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/LavaHomeSeesawRotator.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/MapPartsUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <JSystem/JMath/JMATrigonometric.hpp>

void LavaHomeSeesawRotator_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)JGeometry::TUtil< f32 >::epsilon();
    (void)-JGeometry::TUtil< f32 >::epsilon();
    (void)0.0f;
    (void)0.5f;
    (void)2.0f;
    (void)-HALF_PI;
    (void)HALF_PI;
}

f32 JMAAcosRadian_dummy(f32 f) {
    return JMAAcosRadian(f);
}

namespace {
    const f32 sAngularVelocityFric = 0.99f;
    const f32 sAngularAccelHipDrop = 40.0f;
    const f32 sCollisionEfficiency = -0.5f;
    const f32 sAngularSpeedMin = 0.01f;
    const s32 sMoveStartFrame = 2;
    const f32 sGroundOnAngularSpeed = 0.7f;
    const s32 sGroundOnDownTime = 5;
    const s32 sGroundOnReboundTime = 8;
    const f32 sInertiaConstDefault = 1000.0f;
};  // namespace

namespace NrvLavaHomeSeesawRotator {
    NEW_NERVE(HostTypeWait, LavaHomeSeesawRotator, Wait);
    NEW_NERVE(HostTypeMoveStart, LavaHomeSeesawRotator, MoveStart);
    NEW_NERVE(HostTypeMove, LavaHomeSeesawRotator, Move);
    NEW_NERVE(HostTypeStay, LavaHomeSeesawRotator, Stay);
    NEW_NERVE(HostTypeHipDrop, LavaHomeSeesawRotator, HipDrop);
};  // namespace NrvLavaHomeSeesawRotator

LavaHomeSeesawRotator::LavaHomeSeesawRotator(LiveActor* pHost, const char* pMoveSound, f32 moveSoundSpeed)
    : MapPartsRotatorBase(pHost, "シーソー(1軸)"), mAngularSpeedMax(), mInertia(::sInertiaConstDefault), mRotateAngle(), mRestoreForce(),
      mIsHipDropped(), mRotateAxis(0.0f, 0.0f, 1.0f), mAngularVelocity(), mInitialUp(0.0f, 1.0f, 0.0f), mMoveSound(pMoveSound),
      mMoveSoundSpeed(moveSoundSpeed) {
    mRotateMtx.identity();
    mRotateQuat.set(0.0f, 0.0f, 0.0f, 1.0f);
}

MapPartsRotatorBase::~MapPartsRotatorBase() {
}

void LavaHomeSeesawRotator::init(const JMapInfoIter& rIter) {
    initNerve(GET_NERVE(LavaHomeSeesawRotator, HostTypeWait));

    MR::getMapPartsArgRotateSpeed(&mAngularSpeedMax, rIter);
    mAngularSpeedMax *= 0.01f;
    MR::getMapPartsArgRotateAngle(&mRotateAngle, rIter);
    s32 inertia = 0;
    MR::getMapPartsArgRotateAccelType(&inertia, rIter);

    if (inertia > 0) {
        mInertia = inertia;
    }

    s32 rotateStopTime = 0;
    MR::getMapPartsArgRotateStopTime(&rotateStopTime, rIter);
    mRestoreForce = rotateStopTime;

    TPos3f baseMtx;
    baseMtx.set(mHost->getBaseMtx());
    baseMtx.getYDir(mInitialUp);
    MR::normalize(&mInitialUp);
}

void LavaHomeSeesawRotator::start() {
    mRotateMtx.setRotateDegree(mHost->mRotation);

    TPos3f baseMtx;
    baseMtx.identity();
    baseMtx.set(mHost->getBaseMtx());
    baseMtx.getZDir(mRotateAxis);
    MR::normalize(&mRotateAxis);

    f32 angle = 0.0f;
    calcRotatedAngle(&angle, mRotateMtx);

    if (isAngleLimited() && mRotateAngle < angle) {
        TRot3f reboundMtx;
        reboundMtx.identity();
        reboundMtx.setRotate(mRotateAxis, 1.05f * MR::toRadian(angle - mRotateAngle));
        mRotateMtx.concat(reboundMtx);
    }

    TVec3f euler;
    mRotateMtx.getEulerXYZ(euler);
    mRotateQuat.setEuler(euler);

    mAngularVelocity = ::sAngularSpeedMin;
    setNerve(GET_NERVE(LavaHomeSeesawRotator, HostTypeStay));
}

void LavaHomeSeesawRotator::end() {
    setNerve(GET_NERVE(LavaHomeSeesawRotator, HostTypeWait));
}

bool LavaHomeSeesawRotator::receiveMsg(u32 msg) {
    if (MR::isMsgPlayerHipDropFloor(msg)) {
        if (!MR::isOnPlayer(MR::getBodySensor(mHost))) {
            return false;
        }

        mIsHipDropped = true;

        if (tryHipDrop()) {
            return true;
        }
    }

    return false;
}

void LavaHomeSeesawRotator::exeWait() {
}

void LavaHomeSeesawRotator::exeMoveStart() {
    if (isFirstStep()) {
        TRot3f baseMtx;
        baseMtx.identity();
        baseMtx.set(mHost->getBaseMtx());

        baseMtx.getZDir(mRotateAxis);
        MR::normalize(&mRotateAxis);
        MR::shakeCameraWeak();
        MR::startSound(mHost, "SE_OJ_LAVA_FLOATER_ON_METAL");
    }

    if (isStep(::sGroundOnReboundTime)) {
        setNerve(GET_NERVE(LavaHomeSeesawRotator, HostTypeMove));
    } else {
        updateRotateMtx();
        soundMoveSE();
    }
}

void LavaHomeSeesawRotator::exeMove() {
    updateRotateMtx();
    soundMoveSE();
}

void LavaHomeSeesawRotator::exeStay() {
    if (MR::isOnPlayer(MR::getBodySensor(mHost))) {
        setNerve(GET_NERVE(LavaHomeSeesawRotator, HostTypeMoveStart));
    } else {
        updateRotateMtx();
        soundMoveSE();
    }
}

void LavaHomeSeesawRotator::exeHipDrop() {
    if (isFirstStep()) {
        addForceHipDrop();
    }

    updateRotateMtx();

    if (!mIsHipDropped) {
        setNerve(GET_NERVE(LavaHomeSeesawRotator, HostTypeMove));
        mIsHipDropped = false;
    } else {
        mIsHipDropped = false;
    }
}

void LavaHomeSeesawRotator::updateRotateMtx() {
    updateVelocity();

    if (!tryStay() && !tryRebound()) {
        f32 deltaAngle = mAngularVelocity;

        if (isNerve(GET_NERVE(LavaHomeSeesawRotator, HostTypeMoveStart))) {
            if (getStep() <= ::sGroundOnDownTime) {
                deltaAngle = ::sGroundOnAngularSpeed * MR::sign(mAngularVelocity);
            } else if (getStep() > ::sGroundOnDownTime && getStep() <= ::sGroundOnReboundTime) {
                deltaAngle = ::sGroundOnAngularSpeed * -MR::sign(mAngularVelocity);
            }
        }

        TQuat4f rotate;
        rotate.set(0.0f, 0.0f, 0.0f, 1.0f);
        rotate.setRotate(mRotateAxis, MR::toRadian(deltaAngle));

        mRotateQuat.mult(rotate);
        mRotateQuat.normalize();
        mRotateMtx.setQuat(mRotateQuat);
    }
}

void LavaHomeSeesawRotator::updateVelocity() {
    if (MR::isOnPlayer(MR::getBodySensor(mHost))) {
        TVec3f gravity;
        MR::calcGravityVector(mHost, &gravity, nullptr, 0);

        TVec3f playerDir(*MR::getPlayerPos() - mHost->mPosition);
        TVec3f torque;
        torque.cross(playerDir, gravity);

        f32 direction = MR::sign(torque.dot(mRotateAxis));
        mAngularVelocity += direction * (getDistanceFromRotAxis() / (0.1f * mInertia));
    }

    if (!isNerve(GET_NERVE(LavaHomeSeesawRotator, HostTypeMoveStart)) && getStep() > ::sMoveStartFrame) {
        updateRestoreForce();

        if (!isNerve(GET_NERVE(LavaHomeSeesawRotator, HostTypeStay))) {
            mAngularVelocity *= ::sAngularVelocityFric;
        }
    }

    clampAngularSpeed();
}

void LavaHomeSeesawRotator::updateRestoreForce() {
    TRot3f baseMtx;
    baseMtx.identity();
    baseMtx.set(mHost->getBaseMtx());
    TVec3f up;
    baseMtx.getYDir(up);

    MR::normalize(&up);
    TVec3f torque;
    torque.cross(up, mInitialUp);

    f32 dot = torque.dot(mRotateAxis);
    f32 direction = MR::sign(dot);
    f32 restoreForce = 0.01f * mRestoreForce;
    mAngularVelocity += (direction * restoreForce) / (0.1f * mInertia);
}

void LavaHomeSeesawRotator::clampAngularSpeed() {
    mAngularVelocity = MR::clamp(mAngularVelocity, -mAngularSpeedMax, mAngularSpeedMax);
}

f32 LavaHomeSeesawRotator::getDistanceFromRotAxis() const {
    TVec3f position(mHost->mPosition);
    TVec3f offset(*MR::getPlayerPos() - position);

    TPos3f baseMtx;
    baseMtx.identity();
    baseMtx.set(mHost->getBaseMtx());
    TVec3f axis;
    baseMtx.getXDir(axis);

    TVec3f projection;
    projection.scale(axis.dot(offset), axis);
    return 0.001f * projection.length();
}

void LavaHomeSeesawRotator::addForceHipDrop() {
    TVec3f gravity;
    MR::calcGravityVector(mHost, &gravity, nullptr, 0);

    TVec3f offset(*MR::getPlayerPos() - mHost->mPosition);
    TVec3f torque;
    torque.cross(offset, gravity);

    f32 direction = MR::sign(torque.dot(mRotateAxis));
    f32 inertia = 0.1f * mInertia;
    mAngularVelocity += direction * (::sAngularAccelHipDrop * getDistanceFromRotAxis() / inertia);
}

bool LavaHomeSeesawRotator::isAngleLimited() const {
    return 0.0f < mRotateAngle;
}

bool LavaHomeSeesawRotator::isGoingToReachTargetAngle() const {
    if (!isAngleLimited()) {
        return false;
    }

    TPos3f rotation;
    rotation.identity();
    rotation.setRotate(mRotateAxis, PI_180 * mAngularVelocity);
    rotation.concat(rotation, mRotateMtx);

    f32 angle = 0.0f;
    calcRotatedAngle(&angle, rotation);
    return mRotateAngle <= angle;
}

void LavaHomeSeesawRotator::calcRotatedAngle(f32* pAngle, const TPos3f& rMtx) const {
    TVec3f up;
    rMtx.getYDir(up);
    MR::normalize(&up);
    TVec3f gravityUp;
    TVec3f gravity;
    MR::calcGravityVector(mHost, &gravity, nullptr, 0);

    if (!gravity.isZero()) {
        TVec3f negGravity(-gravity);
        gravityUp.set(negGravity);
    } else {
        gravityUp.set< f32 >(0.0f, 1.0f, 0.0f);
    }

    MR::normalize(&gravityUp);
    TVec3f normal(gravityUp);
    f32 cosine = up.dot(normal);

    if (MR::isNearZero(1.0f - cosine)) {
        cosine = 1.0f;
    }

    if (MR::isNearZero(cosine)) {
        cosine = 0.0f;
    }

    *pAngle = _180_PI * MR::acos(cosine);
}

bool LavaHomeSeesawRotator::tryHipDrop() {
    if (isNerve(GET_NERVE(LavaHomeSeesawRotator, HostTypeMove)) || isNerve(GET_NERVE(LavaHomeSeesawRotator, HostTypeMoveStart)) ||
        isNerve(GET_NERVE(LavaHomeSeesawRotator, HostTypeStay))) {
        setNerve(GET_NERVE(LavaHomeSeesawRotator, HostTypeHipDrop));
        return true;
    }

    return false;
}

bool LavaHomeSeesawRotator::tryStay() {
    if (isNerve(GET_NERVE(LavaHomeSeesawRotator, HostTypeStay))) {
        return false;
    }

    if (isNerve(GET_NERVE(LavaHomeSeesawRotator, HostTypeMoveStart))) {
        return false;
    }

    if (MR::abs(mAngularVelocity) < ::sAngularSpeedMin) {
        TRot3f baseMtx;
        baseMtx.identity();
        baseMtx.set(mHost->getBaseMtx());

        TVec3f up;
        baseMtx.getYDir(up);
        MR::normalize(&up);

        TVec3f crossVec;
        crossVec.cross(up, mInitialUp);
        f32 dot = crossVec.dot(mRotateAxis);

        if (MR::isNearZero(dot)) {
            setNerve(GET_NERVE(LavaHomeSeesawRotator, HostTypeStay));
            return true;
        }
    }

    return false;
}

bool LavaHomeSeesawRotator::tryRebound() {
    if (isGoingToReachTargetAngle()) {
        mAngularVelocity *= ::sCollisionEfficiency;
        return true;
    }

    return false;
}

void LavaHomeSeesawRotator::soundMoveSE() {
    if (mMoveSound == nullptr) {
        return;
    }

    if (mMoveSoundSpeed < MR::abs(mAngularVelocity)) {
        MR::startLevelSound(mHost, mMoveSound);
    }
}

LavaHomeSeesawRotator::~LavaHomeSeesawRotator() {
}
