#pragma once

#include "J3DGraphBase/J3DTransform.h"

class J3DJoint
{
public:
    J3DJoint();

    void appendChild(J3DJoint *);
    void entryIn();
    void recursiveCalc();
    void setMtxType(u8);

    u32 _0;
    u32 _4;
    u32 _8;
    J3DJoint* mChild; // _C
    J3DJoint* mNext; // _10
    u16 mMatrixIdx; // _14
    u8 mMtxTypeFlag; // _16
    u8 _17; // padding?
    J3DTransformInfo mTransformInfo; // _18
    u32 _38;
    Vec _3C;
    Vec _48;
    u32 _54;
    u32 _58;
};

bool checkScaleOne(const Vec &);