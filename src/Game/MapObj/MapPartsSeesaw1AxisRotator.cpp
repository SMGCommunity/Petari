#include <revolution/types.h>

f32 JMAAcosRadian(f32) NO_INLINE;

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/MapObj/MapPartsSeesaw1AxisRotator.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/MapPartsUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/VectorUtil.hpp"

namespace {
    const f32 sAngularVelocityFric = 0.99f;
    const f32 sAngularAccelHipDrop = 40.0f;
    const f32 sCollisionEfficiency = -0.5f;
    const f32 sAngularSpeedMin = 0.1f;
    const s32 sMoveStartFrame = 2;
    const f32 sInertiaConstDefault = 1000.0f;
}  // namespace

namespace NrvMapPartsSeesaw1AxisRotator {
    NEW_NERVE(HostTypeWait, MapPartsSeesaw1AxisRotator, Wait);
    NEW_NERVE(HostTypeMoveStart, MapPartsSeesaw1AxisRotator, Move);
    NEW_NERVE(HostTypeMove, MapPartsSeesaw1AxisRotator, Move);
    NEW_NERVE(HostTypeStay, MapPartsSeesaw1AxisRotator, Stay);
    NEW_NERVE(HostTypeHipDrop, MapPartsSeesaw1AxisRotator, HipDrop);
}  // namespace NrvMapPartsSeesaw1AxisRotator

MapPartsSeesaw1AxisRotator::MapPartsSeesaw1AxisRotator(LiveActor* pHost, const char* pMoveSound, f32 moveSoundSpeed)
    : MapPartsRotatorBase(pHost, "シーソー(1軸)"), mAngularSpeedMax(), mInertiaConst(sInertiaConstDefault), mRotateAngle(), mRestoreForce(),
      mHipDrop(), mRotateAxis(0.0f, 0.0f, 1.0f), mAngularVelocity(), mAngularAccel(), mInitialUp(0.0f, 1.0f, 0.0f), mMoveSound(pMoveSound),
      mMoveSoundSpeed(moveSoundSpeed) {
    mRotateMtx.identity();
}

void MapPartsSeesaw1AxisRotator::init(const JMapInfoIter& rIter) {
    initNerve(GET_NERVE(MapPartsSeesaw1AxisRotator, HostTypeWait));
    MR::getMapPartsArgRotateSpeed(&mAngularSpeedMax, rIter);
    mAngularSpeedMax *= 0.01f;
    MR::getMapPartsArgRotateAngle(&mRotateAngle, rIter);
    s32 inertia = 0;
    MR::getMapPartsArgRotateAccelType(&inertia, rIter);

    if (inertia > 0) {
        mInertiaConst = inertia;
    }

    s32 restoreForce = 0;
    MR::getMapPartsArgRotateStopTime(&restoreForce, rIter);
    mRestoreForce = restoreForce;

    TPos3f baseMtx;
    baseMtx.set(mHost->getBaseMtx());
    baseMtx.getYDir(mInitialUp);
    MR::normalize(&mInitialUp);
}

void MapPartsSeesaw1AxisRotator::start() {
    TPos3f rotateMtx;
    rotateMtx.identity();
    rotateMtx.setRotateDegree(mHost->mRotation);
    mRotateMtx.set(rotateMtx);

    TPos3f baseMtx;
    baseMtx.identity();
    baseMtx.set(mHost->getBaseMtx());
    baseMtx.getZDir(mRotateAxis);
    MR::normalize(&mRotateAxis);

    f32 angle = 0.0f;
    calcRotatedAngle(&angle, mRotateMtx);
    bool hasAngleLimit = isAngleLimited();

    if (hasAngleLimit && mRotateAngle < angle) {
        TPos3f correction;
        correction.identity();
        correction.setRotate(mRotateAxis, 1.05f * (PI_180 * (angle - mRotateAngle)));
        mRotateMtx.concat(correction);
    }

    mAngularVelocity = 0.0f;
    mAngularAccel = 0.0f;
    setNerve(GET_NERVE(MapPartsSeesaw1AxisRotator, HostTypeStay));
}

void MapPartsSeesaw1AxisRotator::end() {
    mAngularVelocity = 0.0f;
    mAngularAccel = 0.0f;
    setNerve(GET_NERVE(MapPartsSeesaw1AxisRotator, HostTypeWait));
}

bool MapPartsSeesaw1AxisRotator::receiveMsg(u32 msg) {
    if (msg == ACTMES_PLAYER_HIP_DROP_FLOOR) {
        if (!MR::isOnPlayer(MR::getBodySensor(mHost))) {
            return false;
        }

        mHipDrop = true;

        if (tryHipDrop()) {
            return true;
        }
    }

    return false;
}

void MapPartsSeesaw1AxisRotator::exeMove() {
    if (isNerve(GET_NERVE(MapPartsSeesaw1AxisRotator, HostTypeMoveStart)) && sAngularSpeedMin < MR::abs(mAngularVelocity)) {
        setNerve(GET_NERVE(MapPartsSeesaw1AxisRotator, HostTypeMove));
        return;
    }

    rotate();

    if (mMoveSound != nullptr && mMoveSoundSpeed < MR::abs(mAngularVelocity)) {
        MR::startLevelSound(mHost, mMoveSound);
    }
}

void MapPartsSeesaw1AxisRotator::exeStay() {
    if (isFirstStep()) {
        mAngularAccel = 0.0f;
    }

    if (MR::isOnPlayer(MR::getBodySensor(mHost)) || mAngularAccel != 0.0f) {
        setNerve(GET_NERVE(MapPartsSeesaw1AxisRotator, HostTypeMoveStart));
    }
}

void MapPartsSeesaw1AxisRotator::exeHipDrop() {
    if (isFirstStep()) {
        addForceHipDrop();
    }

    rotate();

    if (!mHipDrop) {
        setNerve(GET_NERVE(MapPartsSeesaw1AxisRotator, HostTypeMove));
        mHipDrop = false;
        return;
    }

    mHipDrop = false;
}

void MapPartsSeesaw1AxisRotator::rotate() {
    TPos3f baseMtx;
    baseMtx.identity();
    baseMtx.set(mHost->getBaseMtx());
    baseMtx.getZDir(mRotateAxis);
    mAngularVelocity += mAngularAccel;
    updateVelocity();

    if (mAngularVelocity == 0.0f && mAngularAccel == 0.0f) {
        setNerve(GET_NERVE(MapPartsSeesaw1AxisRotator, HostTypeStay));
        return;
    }

    mAngularAccel = 0.0f;

    if (isGoingToReachTargetAngle()) {
        mAngularVelocity *= sCollisionEfficiency;
        return;
    }

    TPos3f rotation;
    rotation.identity();
    rotation.setRotate(mRotateAxis, PI_180 * mAngularVelocity);
    rotation.concat(rotation, mRotateMtx);
    mRotateMtx.set(rotation);
    TVec3f angles;
    mRotateMtx.getEuler(angles);
    TVec3f& rotationAngles = mHost->mRotation;
    rotationAngles.set(angles * _180_PI);
}

void MapPartsSeesaw1AxisRotator::updateVelocity() {
    if (MR::isOnPlayer(MR::getBodySensor(mHost))) {
        TVec3f gravity;
        MR::calcGravityVector(mHost, &gravity, nullptr, 0);
        TVec3f offset(*MR::getPlayerPos() - mHost->mPosition);
        TVec3f torque;
        torque.cross(offset, gravity);
        f32 direction = MR::sign(torque.dot(mRotateAxis));
        mAngularVelocity += direction * (getDistanceFromRotAxis() / (0.1f * mInertiaConst));
    }

    if (!isNerve(GET_NERVE(MapPartsSeesaw1AxisRotator, HostTypeMoveStart)) && getStep() > sMoveStartFrame) {
        updateRestoreForce();
        mAngularVelocity *= sAngularVelocityFric;
    }

    clampAngularSpeed();
}

void MapPartsSeesaw1AxisRotator::updateRestoreForce() {
    TPos3f baseMtx;
    baseMtx.identity();
    baseMtx.set(mHost->getBaseMtx());
    TVec3f up;
    baseMtx.getYDir(up);
    MR::normalize(&up);
    TVec3f torque;
    torque.cross(up, mInitialUp);
    f32 force = torque.dot(mRotateAxis);

    if (MR::isNearZero(force) && __fabsf(mAngularVelocity) <= sAngularSpeedMin) {
        mAngularVelocity = 0.0f;
        return;
    }

    f32 direction = MR::sign(force);
    f32 restoreForce = 0.01f * mRestoreForce;
    mAngularVelocity += (direction * restoreForce) / (0.1f * mInertiaConst);
}

void MapPartsSeesaw1AxisRotator::clampAngularSpeed() {
    mAngularVelocity = MR::clamp(mAngularVelocity, -mAngularSpeedMax, mAngularSpeedMax);
}

f32 MapPartsSeesaw1AxisRotator::getDistanceFromRotAxis() const {
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

void MapPartsSeesaw1AxisRotator::addForceHipDrop() {
    TVec3f gravity;
    MR::calcGravityVector(mHost, &gravity, nullptr, 0);
    TVec3f offset(*MR::getPlayerPos() - mHost->mPosition);
    TVec3f torque;
    torque.cross(offset, gravity);
    f32 direction = MR::sign(torque.dot(mRotateAxis));
    f32 inertia = 0.1f * mInertiaConst;
    mAngularVelocity += direction * ((sAngularAccelHipDrop * getDistanceFromRotAxis()) / inertia);
}

bool MapPartsSeesaw1AxisRotator::isGoingToReachTargetAngle() const {
    bool hasAngleLimit = isAngleLimited();

    if (!hasAngleLimit) {
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

void MapPartsSeesaw1AxisRotator::calcRotatedAngle(f32* pAngle, const TPos3f& rMtx) const {
    TVec3f up;
    rMtx.getYDir(up);
    MR::normalize(&up);
    TVec3f gravityUp;
    TVec3f gravity;
    MR::calcGravityVector(mHost, &gravity, nullptr, 0);

    if (!gravity.isZero()) {
        TVec3f oppositeGravity(-gravity);
        gravityUp.set(oppositeGravity);
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

    *pAngle = _180_PI * JMAAcosRadian(cosine);
}

bool MapPartsSeesaw1AxisRotator::tryHipDrop() {
    if (isNerve(GET_NERVE(MapPartsSeesaw1AxisRotator, HostTypeMove)) || isNerve(GET_NERVE(MapPartsSeesaw1AxisRotator, HostTypeMoveStart)) ||
        isNerve(GET_NERVE(MapPartsSeesaw1AxisRotator, HostTypeStay))) {
        setNerve(GET_NERVE(MapPartsSeesaw1AxisRotator, HostTypeHipDrop));
        return true;
    }

    return false;
}

MapPartsSeesaw1AxisRotator::~MapPartsSeesaw1AxisRotator() {
}

bool MapPartsSeesaw1AxisRotator::isWorking() const {
    return true;
}
