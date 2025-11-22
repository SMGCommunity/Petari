#pragma once

#include <revolution.h>
#include "JSystem/J3DGraphBase/J3DVertex.hpp"

class J3DMtxBuffer;
class JUTNameTab;
class J3DModelHierarchy;
class J3DJoint;

class J3DJointTree {
public:
    J3DJointTree();

    virtual void calc(J3DMtxBuffer*, const Vec&, const Mtx&);
    virtual ~J3DJointTree();

    J3DModelHierarchy* mHierarchy;     // 0x04
    u32                mFlags;         // 0x08
    u32                mModelDataType; // 0x0C
    u32                _10;
    u32                _14;
    J3DJoint**         mJointNode;            // 0x18
    u16                mJointNum;             // 0x1C
    u16                mWEvlpMtxNum;          // 0x1E
    u8*                mWEvlpMixMtxNum;       // 0x20
    u16*               mWEvlpMixMtxIndex;     // 0x24
    f32*               mWEvlpMixWeight;       // 0x28
    Mtx*               mInvJointMtx;          // 0x2C
    u16*               mWEvlpImportantMtxIdx; // 0x30
    J3DDrawMtxData     mMatrixData;           // 0x34
    u32                _40;
    JUTNameTab*        mJointName; // 0x44
};
