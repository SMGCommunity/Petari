#pragma once

#include <JSystem/JGeometry.hpp>

class IKJoint {
public:
    IKJoint();

    void setRootBoneLength(f32);
    void setMiddleBoneLength(f32);
    void setFirstPose(const TVec3f&, const TVec3f&);
    static s32 checkReachIKTarget(f32, f32, f32);
    static f32 calcIKRootAngleCosign(f32, f32, f32);

    void update(MtxPtr, MtxPtr, MtxPtr, const TVec3f&);

    void getRootJointPosition(TVec3f*) const;
    void getEndJointPosition(TVec3f*) const;

    f32 getMaxLimitDistance() const;
    f32 getMinLimitDistance() const;

    void updateByLocalRootAndWorldTarget(const TPos3f&, const TVec3f&, const TVec3f&);

    s32 updateByLocalRootAndDirection(const TPos3f&, const TVec3f&, const TVec3f&);

    s32 updateByUpVector(const TVec3f&, const TVec3f&, const TVec3f&);

    s32 updateByDistanceOnly(f32);

    void calcToTargetMatrixByFirstPose(TPos3f* pMatrix, const TPos3f& rRootMatrix, const TVec3f& rRootPosition, const TVec3f& rDirection) const;

    /* 0x0 */ TPos3f _0;
    /* 0x30 */ TPos3f _30;
    /* 0x60 */ TPos3f _60;
    /* 0x90 */ f32 mRootBoneLength;
    /* 0x94 */ f32 mMiddleBoneLength;
};
