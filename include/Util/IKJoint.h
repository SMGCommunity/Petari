#pragma once

#include "JSystem/JGeometry/TMatrix34.h"
#include "smg.h"

class IKJoint
{
public:
    IKJoint();

    void setRootBoneLength(f32);
    void setMiddleBoneLength(f32);

    s32 checkReachIKTarget(f32, f32, f32);
    f32 calcIKRootAngleCosign(f32, f32, f32);

    JGeometry::TPosition3<JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > > _0;
    JGeometry::TPosition3<JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > > _30;
    JGeometry::TPosition3<JGeometry::TMatrix34<JGeometry::SMatrix34C<f32> > > _60;

private:
    f32 mRootBoneLength; // _90
    f32 mMiddleBoneLength; // _94
};