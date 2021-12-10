#pragma once

#include <revolution.h>
#include "JSystem/J3DGraphBase/J3DVertex.h"

class J3DMtxBuffer;

class J3DJointTree {
public:
    J3DJointTree();

    virtual void calc(J3DMtxBuffer *, const Vec &, const Mtx &);
    virtual ~J3DJointTree();

    u32 _4;
    u32 _8;
    u32 _C;
    u32 _10;
    u32 _14;
    u32 _18;
    u16 _1C;
    u16 _1E;
    u32 _20;
    u32 _24;
    u32 _28;
    u32 _2C;
    u32 _30;
    J3DDrawMtxData mMatrixData; // _34
    u32 _40;
    u32 _44;
};