#pragma once

#include "JSystem/J3DGraphBase/J3DTransform.hpp"
#include <revolution.h>

class J3DMtxCalc;
class J3DMaterial;

class J3DJoint {
public:
    void* mJointData;      // 0x0
    void* mJointCallback;  // 0x4
    void* _8;
    J3DJoint* mChild;                 // 0x0C
    J3DJoint* mYoung;                 // 0x10
    u16 mJointNo;                     // 0x14
    u8 mKind;                         // 0x16
    u8 mScaleComp;                    // 0x17
    J3DTransformInfo mTransformInfo;  // 0x18
    f32 mBoundingSphereRadius;        // 0x38
    Vec mMin;                         // 0x3C
    Vec mMax;                         // 0x48
    J3DMtxCalc* mMtxCalc;             // 0x54
    J3DMaterial* mMesh;               // 0x58
};
