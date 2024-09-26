#pragma once

#include "JSystem/JGeometry.hpp"

class IKJoint {
public:
    IKJoint();

    void setRootBoneLength(f32);
    void setMiddleBoneLength(f32);
    void setFirstPose(const TVec3f &, const TVec3f &);
    s32 checkReachIKTarget(f32, f32, f32);
    f32 calcIKRootAngleCosign(f32, f32, f32);

    void update(MtxPtr, MtxPtr, MtxPtr, const TVec3f &);

    TPos3f _0;
    TPos3f _30;
    TPos3f _60;
    f32 mRootBoneLength;    // 0x90
    f32 mMiddleBoneLength;  // 0x94
};