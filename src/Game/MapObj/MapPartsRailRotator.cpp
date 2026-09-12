#include "Game/MapObj/MapPartsRailRotator.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/MapPartsUtil.hpp"
#include "Game/Util/MathUtil.hpp"

namespace NrvMapPartsRailRotator {
    NEW_NERVE(HostTypeWait, MapPartsRailRotator, Wait);
    NEW_NERVE(HostTypeRotateAtPoint, MapPartsRailRotator, Rotate);
    NEW_NERVE(HostTypeRotateBetweenPoints, MapPartsRailRotator, Rotate);
    NEW_NERVE(HostTypeDone, MapPartsRailRotator, Done);
}  // namespace NrvMapPartsRailRotator

MapPartsRailRotator::MapPartsRailRotator(LiveActor* pHost)
    : MapPartsFunction(pHost, "レイル回転"), mRotateAxis(), mRotateType(), mRotateSpeed(), mTargetAngle(), mAngle(), mHostRotateMtx() {
    _2C.identity();
    _5C.identity();
}

void MapPartsRailRotator::init(const JMapInfoIter&) {
    initNerve(GET_NERVE(MapPartsRailRotator, HostTypeWait));
}

void MapPartsRailRotator::initWithRotateMtx(const JMapInfoIter& rIter, MtxPtr pMtx) {
    mHostRotateMtx = pMtx;
    init(rIter);
}

void MapPartsRailRotator::start() {
}

void MapPartsRailRotator::end() {
    _2C.identity();
    _5C.identity();
    mAngle = 0.0f;
    setNerve(GET_NERVE(MapPartsRailRotator, HostTypeWait));
}

bool MapPartsRailRotator::isWorking() const {
    return 0.0f != mRotateSpeed && 0.0f < mTargetAngle;
}

bool MapPartsRailRotator::hasRotation(s32 point) const {
    f32 speed = -1.0f;
    MR::getMapPartsArgRailRotateSpeed(&speed, mHost, point);
    f32 angle = 0.0f;
    MR::getMapPartsArgRailRotateAngle(&angle, mHost, point);
    return 0.0f != speed && 0.0f < angle;
}

void MapPartsRailRotator::rotateAtPoint(s32 point) {
    updateInfo(point);

    if (!isWorking()) {
        setNerve(GET_NERVE(MapPartsRailRotator, HostTypeDone));
    } else {
        setNerve(GET_NERVE(MapPartsRailRotator, HostTypeRotateAtPoint));
    }
}

bool MapPartsRailRotator::hasRotationBetweenPoints(s32 point) const {
    if (!hasRotation(point)) {
        return false;
    }

    s32 type = 0;
    MR::getMapPartsArgRailRotateType(&type, mHost, point);
    return type == 1;
}

void MapPartsRailRotator::rotateBetweenPoints(s32 point, f32 time) {
    updateInfo(point);

    if (!isWorking()) {
        setNerve(GET_NERVE(MapPartsRailRotator, HostTypeDone));
    } else {
        mRotateSpeed = (mTargetAngle / time) * MR::sign(mRotateSpeed);
        setNerve(GET_NERVE(MapPartsRailRotator, HostTypeRotateBetweenPoints));
    }
}

void MapPartsRailRotator::updateHostRotateMtx() {
    if (mHostRotateMtx != nullptr) {
        PSMTXCopy(mHostRotateMtx, _2C.toMtxPtr());
    } else if (mHost->getBaseMtx() != nullptr) {
        _2C.set(mHost->getBaseMtx());
        _2C.zeroTrans();
    } else {
        _2C.setRotateDegree(mHost->mRotation);
    }
}

void MapPartsRailRotator::updateInfo(s32 point) {
    s32 speedCalcType = -1;
    MR::getMapPartsArgSpeedCalcType(&speedCalcType, mHost, point);

    if (MR::isMapPartsRailSpeedCalcTypeTime(speedCalcType)) {
        s32 time = 0;
        MR::getMapPartsArgRailRotateTime(&time, mHost, point);
        f32 angle = 0.0f;
        MR::getMapPartsArgRailRotateAngle(&angle, mHost, point);
        mRotateSpeed = angle / time;
    } else {
        f32 speed = -1.0f;
        MR::getMapPartsArgRailRotateSpeed(&speed, mHost, point);
        mRotateSpeed = 0.01f * speed;
    }

    f32 angle = -1.0f;
    MR::getMapPartsArgRailRotateAngle(&angle, mHost, point);
    angle *= getJMapArgAngleFactor();
    mTargetAngle = angle;
    MR::getMapPartsArgRailRotateAxis(&mRotateAxis, mHost, point);
    MR::getMapPartsArgRailRotateType(&mRotateType, mHost, point);

    if (angle < 0.0f) {
        mRotateSpeed = 0.0f;
    }

    mAngle = 0.0f;
    updateHostRotateMtx();
}

bool MapPartsRailRotator::isReachedTargetAngle() const {
    return mTargetAngle <= MR::abs(mAngle);
}

void MapPartsRailRotator::calcRotateAxisDir(AxisType axis, TVec3f* pDir) const {
    switch (axis) {
    case AxisType_X:
        _2C.getXDir(*pDir);
        break;
    case AxisType_Y:
        _2C.getYDir(*pDir);
        break;
    case AxisType_Z:
        _2C.getZDir(*pDir);
        break;
    }
}

void MapPartsRailRotator::updateRotateMtx(AxisType axis, f32 angle) {
    TVec3f direction;
    calcRotateAxisDir(axis, &direction);
    _5C.identity();
    _5C.makeRotate(direction, PI_180 * angle);
    _5C.concat(_5C, _2C);
}

void MapPartsRailRotator::exeRotate() {
    if (isFirstStep()) {
        updateHostRotateMtx();
    }

    mAngle += mRotateSpeed;

    if (isReachedTargetAngle()) {
        updateRotateMtx(static_cast< AxisType >(mRotateAxis), mTargetAngle * MR::sign(mRotateSpeed));
        sendMsgToHost(ACTMES_MAPPARTS_END_ROTATE_AT_POINT);
        setNerve(GET_NERVE(MapPartsRailRotator, HostTypeDone));
    } else {
        updateRotateMtx(static_cast< AxisType >(mRotateAxis), mAngle);
    }
}

f32 MapPartsRailRotator::getJMapArgAngleFactor() const {
    return 1.0f;
}

MapPartsRailRotator::~MapPartsRailRotator() {
}
