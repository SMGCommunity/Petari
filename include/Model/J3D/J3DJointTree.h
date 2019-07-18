#ifndef J3DJOINTTREE_H
#define J3DJOINTTREE_H

#include "types.h"

class J3DMtxBuffer;

class J3DJointTree
{
public:
    J3DJointTree();

    virtual void calc(J3DMtxBuffer *, const Vec &, const Mtx &);
    virtual ~J3DJointTree();

    u32* mRootJoint; // J3DJoint* (_10)
    u32* mCurMtxCalc; // _14
    u32** mJoints; // J3DJoint** (_18)


};

#endif // J3DJOINTTREE_H