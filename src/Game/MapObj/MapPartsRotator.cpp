#include "Game/MapObj/MapPartsRotator.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/Spine.hpp"
#include "Game/MapObj/MapParts.hpp"
#include "Game/MapObj/MapPartsRailRotator.hpp"
#include "Game/Util.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MathUtil.hpp"

void MapPartsRotator_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)-1.0f;
    (void)0.0010000000474974513f;
    (void)0.01745329238474369f;
    (void)360.0f;
    (void)0.009999999776482582f;
    (void)9.999999747378752e-06f;
    (void)0.0f;
}

namespace NrvMapPartsRotator {
    NEW_NERVE(HostTypeNeverMove, MapPartsRotator, NeverMove);
    NEW_NERVE(HostTypeWait, MapPartsRotator, Wait);
    NEW_NERVE(HostTypeRotateStart, MapPartsRotator, RotateStart);
    NEW_NERVE(HostTypeRotate, MapPartsRotator, Rotate);
    NEW_NERVE(HostTypeStopAtEnd, MapPartsRotator, StopAtEnd);
};  // namespace NrvMapPartsRotator

MapPartsRotator::MapPartsRotator(LiveActor* pActor) : MapPartsRotatorBase(pActor, "自身回転") {
    _18 = 0.0f;
    mRotateAngle = 0.0f;
    mRotateStopTime = 0;
    mTargetAngle = 0.0f;
    mRotateSpeed = 0.0f;
    mAngle = 0.0f;
    mRotateAxis = AxisType_XAxis;
    mRotateAccelType = 0;
    mRotateType = 1;
    mSignMotionType = 0;
    _A0 = 0.0f;
    mIsOnReverse = false;
    mBaseHostMtx.identity();
    mRotateMtx.identity();
    updateBaseHostMtx();
    mRotateMtx.set(mBaseHostMtx);
}

void MapPartsRotator::init(const JMapInfoIter& rIter) {
    MR::getMapPartsArgRotateAngle(&mRotateAngle, rIter);
    MR::getMapPartsArgRotateAxis(&mRotateAxis, rIter);
    MR::getMapPartsArgRotateAccelType(&mRotateAccelType, rIter);
    initRotateSpeed(rIter);
    MR::getMapPartsArgRotateStopTime(&mRotateStopTime, rIter);
    MR::getMapPartsArgRotateType(&mRotateType, rIter);
    MR::getMapPartsArgSignMotionType(&mSignMotionType, rIter);

    if (MR::isNearZero(_18)) {
        initNerve(GET_NERVE(MapPartsRotator, HostTypeNeverMove));
    } else {
        initNerve(GET_NERVE(MapPartsRotator, HostTypeWait));
    }

    f32 angle;

    if (isRotateClockwise()) {
        angle = mRotateAngle;
    } else {
        angle = -mRotateAngle;
    }

    mTargetAngle = angle;
}

void MapPartsRotator::start() {
    updateTargetAngle();
    updateRotateMtx((AxisType)mRotateAxis, mAngle);
    setNerve(GET_NERVE(MapPartsRotator, HostTypeRotate));
}

void MapPartsRotator::end() {
    setNerve(GET_NERVE(MapPartsRotator, HostTypeWait));
}

bool MapPartsRotator::isWorking() const {
    if (!isNerve(GET_NERVE(MapPartsRotator, HostTypeNeverMove))) {
        return !isNerve(GET_NERVE(MapPartsRotator, HostTypeWait));
    }

    return false;
}

void MapPartsRotator::startWithSignalMotion() {
    updateBaseHostMtx();
    updateTargetAngle();
    setNerve(GET_NERVE(MapPartsRotator, HostTypeRotateStart));
}

void MapPartsRotator::cancelSignalMotion() {
    mAngle = _A0;
    updateRotateMtx((AxisType)mRotateAxis, _A0);
    setNerve(GET_NERVE(MapPartsRotator, HostTypeWait));
}

void MapPartsRotator::updateBaseHostMtx() {
    mBaseHostMtx.setEuler(mHost->mRotation * (PI / 180.0f));
}

bool MapPartsRotator::isMoving() const {
    return isNerve(GET_NERVE(MapPartsRotator, HostTypeRotate));
}

void MapPartsRotator::updateVelocity() {
    if (MR::isNearZero(mRotateAngle) || isAccelTypeConstantSpeed()) {
        mIsOnReverse = false;
        mRotateSpeed = _18;
        return;
    }

    if (mRotateAccelType != 1) {
        return;
    }

    f32 rotateAccel = (_18 * _18 * MR::sign(_18)) / mRotateAngle;
    bool invert;

    if (isRotateClockwise()) {
        invert = mTargetAngle - (mRotateAngle * 0.5f) <= mAngle;
    } else {
        invert = mAngle <= mTargetAngle + (mRotateAngle * 0.5f);
    }

    if (invert) {
        rotateAccel *= -1.0f;
    }

    f32 oldRotateSpeed = mRotateSpeed;
    mRotateSpeed += rotateAccel;

    if ((oldRotateSpeed >= 0.0f && mRotateSpeed < 0.0f) || (oldRotateSpeed < 0.0f && mRotateSpeed >= 0.0f)) {
        mIsOnReverse = true;
    } else {
        mIsOnReverse = false;
    }
}

void MapPartsRotator::updateAngle() {
    mAngle += mRotateSpeed;

    if (MR::isNearZero(mRotateAngle)) {
        MR::repeatDegree(&mAngle);
    }
}

void MapPartsRotator::updateTargetAngle() {
    if (MR::isNearZero(mRotateAngle)) {
        return;
    }

    if (isRotateClockwise()) {
        mTargetAngle = mAngle + mRotateAngle;
    } else {
        mTargetAngle = mAngle - mRotateAngle;
    }
}

void MapPartsRotator::restartAtEnd() {
    if (mRotateType != 0) {
        if (mRotateType == 1) {
            _18 *= -1.0f;
        }

        updateTargetAngle();

        if (MR::hasMapPartsMoveStartSignMotion(mSignMotionType)) {
            setNerve(GET_NERVE(MapPartsRotator, HostTypeRotateStart));
        } else {
            setNerve(GET_NERVE(MapPartsRotator, HostTypeRotate));
        }
    }
}

void MapPartsRotator::initRotateSpeed(const JMapInfoIter& rIter) {
    if (mRotateAccelType == 2) {
        s32 rotate_time = 0;
        MR::getMapPartsArgRotateTime(&rotate_time, rIter);
        _18 = mRotateAngle / rotate_time;
    } else {
        MR::getMapPartsArgRotateSpeed(&_18, rIter);
        _18 *= 0.01f;
    }

    if (mRotateAngle < 0.0f) {
        _18 = 0.0f;
    }
}

bool MapPartsRotator::isReachedTargetAngle() const {
    if (MR::isNearZero(mRotateAngle)) {
        return false;
    }

    if (isRotateClockwise()) {
        return mTargetAngle <= mAngle;
    } else {
        return mAngle <= mTargetAngle;
    }
}

void MapPartsRotator::updateRotateMtx(AxisType type, f32 angle) {
    TVec3f rotateAxisDir;
    calcRotateAxisDir(type, &rotateAxisDir);
    mRotateMtx.identity();
    mRotateMtx.makeRotateDegree(rotateAxisDir, angle);
    mRotateMtx.concat(mRotateMtx, mBaseHostMtx);
}

void MapPartsRotator::calcRotateAxisDir(AxisType type, TVec3f* pDir) const {
    switch (type) {
    case AxisType_XAxis:
        mBaseHostMtx.getXDir(*pDir);
        break;
    case AxisType_YAxis:
        mBaseHostMtx.getYDir(*pDir);
        break;
    case AxisType_ZAxis:
        mBaseHostMtx.getZDir(*pDir);
        break;
    }
}

void MapPartsRotator::exeNeverMove() {
}

void MapPartsRotator::exeWait() {
}

void MapPartsRotator::exeRotate() {
    updateVelocity();
    updateAngle();

    if (isAccelTypeConstantSpeed() && isReachedTargetAngle()) {
        mAngle = mTargetAngle;
        updateRotateMtx((AxisType)mRotateAxis, mAngle);

        if (mRotateStopTime > 0) {
            setNerve(GET_NERVE(MapPartsRotator, HostTypeStopAtEnd));
        } else {
            restartAtEnd();
        }

        return;
    }

    if (mRotateAccelType == 1 && MR::isNearZero(mRotateSpeed, 0.00001f)) {
        setNerve(GET_NERVE(MapPartsRotator, HostTypeStopAtEnd));
        return;
    }

    updateRotateMtx((AxisType)mRotateAxis, mAngle);
}

void MapPartsRotator::exeRotateStart() {
    if (isFirstStep()) {
        _A0 = mAngle;
    }

    mAngle += 0.5f * (getStep() / 3 % 2 == 0 ? 1.0f : -1.0f);
    updateRotateMtx((AxisType)mRotateAxis, mAngle);

    if (isStep(MapParts::getMoveStartSignalTime())) {
        mAngle = _A0;
        updateRotateMtx((AxisType)mRotateAxis, mAngle);
        setNerve(GET_NERVE(MapPartsRotator, HostTypeRotate));
    }
}

void MapPartsRotator::exeStopAtEnd() {
    if (isStep(mRotateStopTime)) {
        restartAtEnd();
    }
}
