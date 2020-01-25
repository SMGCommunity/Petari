#ifndef J3DJOINTTREE_H
#define J3DJOINTTREE_H

#include "types.h"
#include "revolution/vec.h"

class J3DMtxBuffer;

class J3DJointTree
{
public:
    J3DJointTree();

    virtual void calc(J3DMtxBuffer *, const Vec &, const Mtx &);
    virtual ~J3DJointTree();

    u32 _0;
    u32 _4;
    u32 _8;
    u32 _C;
    u32* mRootJoint; // J3DJoint* (_10)
    u32* mCurMtxCalc; // _14
    u32** mJoints; // J3DJoint** (_18)
    u8 _1C[0x48-0x1C];
};

#endif // J3DJOINTTREE_H