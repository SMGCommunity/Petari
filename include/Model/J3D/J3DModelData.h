#ifndef J3DMODELDATA_H
#define J3DMODELDATA_H

#include "types.h"
#include "Model/J3D/J3DJointTree.h"
#include "Model/J3D/J3DMaterialAttach.h"
#include "Model/J3D/J3DVertex.h"

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

#endif // J3DMODELDATA_H