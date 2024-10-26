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

void IKJoint::setFirstPose(const TVec3f &a1, const TVec3f &a2) {
    MR::makeMtxSideUp(&_0, a1, a2);
}

s32 IKJoint::checkReachIKTarget(f32 a1, f32 a2, f32 a3) {
    int res = 0;

    if (a1 > (a2 + a3)) {
        res = 2;
    }
    else {
        if (a1 < __fabs(a2 - a3)) {
            res = 1;
        }
        else {
            return res;
        }
        
    }

    return res;
}

#ifdef NON_MATCHING
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
#endif