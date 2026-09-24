#include "Game/Util/IKJoint.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"

void IKJoint_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)-1.0f;
    (void)2.0f;
    (void)3.0f;
    (void)100.0f;
    (void)-2.0f;
    (void)0.01f;
}

IKJoint::IKJoint() : _0(), _30(), _60(), mRootBoneLength(100.0f), mMiddleBoneLength(100.0f) {
    _0.identity();
    _60.identity();
    _30.identity();
}

void IKJoint::setRootBoneLength(f32 boneLength) {
    mRootBoneLength = boneLength;
}

void IKJoint::setMiddleBoneLength(f32 boneLength) {
    mMiddleBoneLength = boneLength;
}

void IKJoint::setFirstPose(const TVec3f& rSide, const TVec3f& rUp) {
    MR::makeMtxSideUp(&_0, rSide, rUp);
}

s32 IKJoint::checkReachIKTarget(f32 distance, f32 rootLength, f32 middleLength) {
    int result = 0;

    if (distance > (rootLength + middleLength)) {
        result = 2;
    } else {
        if (distance < MR::abs(rootLength - middleLength)) {
            result = 1;
        } else {
            return result;
        }
    }

    return result;
}

f32 IKJoint::calcIKRootAngleCosign(f32 distance, f32 rootLength, f32 middleLength) {
    s32 reach = checkReachIKTarget(distance, rootLength, middleLength);

    if (reach == 1) {
        return 1.0f;
    }

    if (reach == -1) {
        if (rootLength < middleLength) {
            return -1.0f;
        }

        return 1.0f;
    }

    f32 distanceSquared = distance * distance;
    f32 rootLengthSquared = rootLength * rootLength;
    f32 middleLengthSquared = middleLength * middleLength;
    f32 value = middleLengthSquared - distanceSquared - rootLengthSquared;
    value /= -2.0f * rootLength * distance;
    return MR::clamp(value, -1.0f, 1.0f);
}

void IKJoint::update(MtxPtr pRootMatrix, MtxPtr pMiddleMatrix, MtxPtr pEndMatrix, const TVec3f& rTarget) {
    TPos3f rootMatrix;
    TVec3f rootPosition;
    TVec3f middlePosition;
    TVec3f endPosition;
    MR::extractMtxTrans(pRootMatrix, &rootPosition);
    MR::extractMtxTrans(pMiddleMatrix, &middlePosition);
    MR::extractMtxTrans(pEndMatrix, &endPosition);
    mRootBoneLength = rootPosition.distance(middlePosition);
    mMiddleBoneLength = middlePosition.distance(endPosition);

    TVec3f targetDirection(rTarget - rootPosition);
    TVec3f side(endPosition - rootPosition);
    if (MR::normalizeOrZero(&side)) {
        MR::extractMtxXDir(pRootMatrix, &side);
    }

    TVec3f up(middlePosition - rootPosition);
    if (MR::isSameDirection(side, up, 0.01f)) {
        MR::extractMtxYDir(pRootMatrix, &up);
    }

    MR::makeMtxSideUp(&_0, side, up);
    rootMatrix.identity();
    updateByLocalRootAndDirection(rootMatrix, rootPosition, targetDirection);
}

void IKJoint::updateByLocalRootAndWorldTarget(const TPos3f& rRootMatrix, const TVec3f& rRootPosition, const TVec3f& rTarget) {
    TPos3f inverseRoot = rRootMatrix;
    inverseRoot.invert(rRootMatrix);
    TVec3f localTarget;
    inverseRoot.mult(rTarget, localTarget);
    updateByLocalRootAndDirection(rRootMatrix, rRootPosition, localTarget - rRootPosition);
}

s32 IKJoint::updateByLocalRootAndDirection(const TPos3f& rRootMatrix, const TVec3f& rRootPosition, const TVec3f& rDirection) {
    TPos3f matrix;
    TVec3f direction(rDirection);
    f32 distance;
    MR::separateScalarAndDirection(&distance, &direction, direction);
    s32 result = updateByDistanceOnly(distance);
    calcToTargetMatrixByFirstPose(&matrix, rRootMatrix, rRootPosition, direction);
    _30.concat(matrix, _30);
    _60.concat(matrix, _60);
    return result;
}

s32 IKJoint::updateByUpVector(const TVec3f& rRootPosition, const TVec3f& rTarget, const TVec3f& rUp) {
    f32 distance;
    TPos3f matrix;
    TVec3f direction = rTarget - rRootPosition;
    MR::separateScalarAndDirection(&distance, &direction, direction);
    s32 result = updateByDistanceOnly(distance);
    MR::makeMtxSideUpPos(&matrix, direction, rUp, rRootPosition);
    _30.concat(matrix, _30);
    _60.concat(matrix, _60);
    return result;
}

s32 IKJoint::updateByDistanceOnly(f32 distance) {
    s32 result = checkReachIKTarget(distance, mRootBoneLength, mMiddleBoneLength);
    f32 cosine = calcIKRootAngleCosign(distance, mRootBoneLength, mMiddleBoneLength);
    f32 sine = MR::fastSqrtf(1.0f - cosine * cosine);
    TVec3f rootSide(cosine, sine, 0.0f);
    TVec3f rootUp(-sine, cosine, 0.0f);
    _30.identity();
    _30.setXDir(rootSide);
    _30.setYDir(rootUp);

    TVec3f middlePosition(rootSide * mRootBoneLength);
    TVec3f middleSide(TVec3f(distance, 0.0f, 0.0f) - middlePosition);
    MR::normalizeOrZero(&middleSide);
    TVec3f middleUp(-middleSide.y, middleSide.x, 0.0f);
    _60.identity();
    _60.setXDir(middleSide);
    _60.setYDir(middleUp);
    _60.setTrans(middlePosition);
    return result;
}

void IKJoint::calcToTargetMatrixByFirstPose(TPos3f* pMatrix, const TPos3f& rRootMatrix, const TVec3f& rRootPosition, const TVec3f& rDirection) const {
    TPos3f matrix;
    TVec3f side;
    _0.getXDir(side);
    matrix.identity();
    matrix.setRotate(side, rDirection);
    matrix.concat(matrix, _0);
    matrix.setTrans(rRootPosition);
    matrix.concat(rRootMatrix, matrix);
    pMatrix->set(matrix);
}

void IKJoint::getRootJointPosition(TVec3f* pPos) const {
    _30.getTrans(*pPos);
}

void IKJoint::getEndJointPosition(TVec3f* pPos) const {
    TVec3f xDir;
    _60.getXDir(xDir);
    _60.getTrans(*pPos);
    pPos->add(xDir * mMiddleBoneLength);
}

f32 IKJoint::getMinLimitDistance() const {
    return MR::abs(mRootBoneLength - mMiddleBoneLength);
}

f32 IKJoint::getMaxLimitDistance() const {
    return mRootBoneLength + mMiddleBoneLength;
}
