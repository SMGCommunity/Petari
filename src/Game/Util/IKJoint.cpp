#include "Game/Util/IKJoint.hpp"
#include "Game/Util.hpp"

IKJoint::IKJoint() : mRootBoneLength(100.0f), mMiddleBoneLength(100.0f), _0(), _30(), _60() {
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

void IKJoint::setFirstPose(const TVec3f& a1, const TVec3f& a2) {
    MR::makeMtxSideUp(&_0, a1, a2);
}

s32 IKJoint::checkReachIKTarget(f32 a1, f32 a2, f32 a3) {
    int res = 0;

    if (a1 > (a2 + a3)) {
        res = 2;
    } else {
        if (a1 < __fabs(a2 - a3)) {
            res = 1;
        } else {
            return res;
        }
    }

    return res;
}

// regalloc
f32 IKJoint::calcIKRootAngleCosign(f32 a1, f32 a2, f32 a3) {
    s32 targ = checkReachIKTarget(a1, a2, a3);

    if (targ == 1) {
        return 1.0f;
    }

    if (targ == -1) {
        if (a2 < a3) {
            return -1.0f;
        }

        return 1.0f;
    }

    f32 a3_sqr = a3 * a3;
    f32 a2_sqr = a2 * a2;
    f32 a1_sqr = a1 * a2;

    f32 more_val = (a3_sqr - a1_sqr) - a2_sqr;
    f32 val = (more_val / ((-2.0f * a2) * a1));

    if (val < -1.0f) {
        return -1.0f;
    }

    if (val > 1.0f) {
        return 1.0f;
    }

    return val;
}

// IKJoint::update

void IKJoint::updateByLocalRootAndWorldTarget(const TPos3f& a1, const TVec3f& a2, const TVec3f& a3) {
    TPos3f mtx = a1;
    mtx.invert(a1);
    TVec3f stack_14;
    mtx.mult(a3, stack_14);
    updateByLocalRootAndDirection(a1, a2, stack_14 - a2);
}

// IKJoint::updateByLocalRootAndDirection

s32 IKJoint::updateByUpVector(const TVec3f& a1, const TVec3f& a2, const TVec3f& a3) {
    f32 v9;
    TPos3f mtx;
    TVec3f sub = a2 - a1;
    MR::separateScalarAndDirection(&v9, &sub, sub);
    s32 v7 = updateByDistanceOnly(v9);
    MR::makeMtxSideUpPos(&mtx, sub, a3, a1);
    _30.concat(mtx, _30);
    _60.concat(mtx, _60);
    return v7;
}

void IKJoint::getRootJointPosition(TVec3f* pPos) const {
    _30.getTrans(*pPos);
}

void IKJoint::getEndJointPosition(TVec3f* pPos) const {
    TVec3f xDir;
    _60.getXDir(xDir);
    _60.getTrans(*pPos);
    TVec3f scale = (xDir * mMiddleBoneLength);
    JMathInlineVEC::PSVECAdd(pPos, &scale, pPos);
}

f32 IKJoint::getMinLimitDistance() const {
    return __fabsf(mRootBoneLength - mMiddleBoneLength);
}

f32 IKJoint::getMaxLimitDistance() const {
    return mRootBoneLength + mMiddleBoneLength;
}
