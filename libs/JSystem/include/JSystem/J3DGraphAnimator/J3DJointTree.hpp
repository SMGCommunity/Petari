#pragma once

#include "JSystem/J3DGraphBase/J3DVertex.hpp"
#include <revolution.h>

class J3DMtxBuffer;
class JUTNameTab;

struct J3DModelHierarchy {
    /* 0x0 */ u16 mType;
    /* 0x2 */ u16 mValue;
};

class J3DJoint;
class J3DMtxCalc;

class J3DJointTree {
public:
    J3DJointTree();

    virtual void calc(J3DMtxBuffer*, const Vec&, const Mtx&);
    virtual ~J3DJointTree();

    Mtx& getInvJointMtx(int idx) { return mInvJointMtx[idx]; }

    u8 getDrawMtxFlag(u16 idx) const { return mDrawMtxData.mDrawMtxFlag[idx]; }

    u16 getDrawMtxIndex(u16 idx) const { return mDrawMtxData.mDrawMtxIndex[idx]; }

    /* 0x04 */ J3DModelHierarchy* mHierarchy;
    /* 0x08 */ u32 mFlags;
    /* 0x0C */ u32 mModelDataType;
    /* 0x10 */ J3DJoint* mRootNode;
    /* 0x14 */ J3DMtxCalc* mBasicMtxCalc;
    /* 0x18 */ J3DJoint** mJointNodePointer;
    /* 0x1C */ u16 mJointNum;
    /* 0x1E */ u16 mWEvlpMtxNum;
    /* 0x20 */ u8* mWEvlpMixMtxNum;
    /* 0x24 */ u16* mWEvlpMixMtxIndex;
    /* 0x28 */ f32* mWEvlpMixWeight;
    /* 0x2C */ Mtx* mInvJointMtx;
    /* 0x30 */ u16* mWEvlpImportantMtxIdx;
    /* 0x34 */ J3DDrawMtxData mDrawMtxData;
    /* 0x40 */ u32 field_0x40;
    /* 0x44 */ JUTNameTab* mJointName;
};
