#pragma once

#include "J3DGraphBase/J3DJointTree.h"
#include "J3DGraphBase/J3DMaterialAttach.h"
#include "J3DGraphBase/J3DVertex.h"

class J3DShapeTable
{
public:
    inline J3DShapeTable();

    virtual ~J3DShapeTable();

    u32 _4;
    u32 _8;
    u32 _C;
};

class J3DModelData
{
public:
    J3DModelData();

    virtual ~J3DModelData();

    void clear();

    u32 _4;
    u32 mMtxCalcFlags; // _8
    u16 _C;
    u16 _E;
    J3DJointTree mJointTree; // _10
    J3DMaterialTable mMatTable; // _58
    J3DShapeTable mShapeTable; // _78
    J3DVertexData mVertexData; // _88
};