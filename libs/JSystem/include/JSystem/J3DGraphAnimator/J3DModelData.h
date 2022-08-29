#pragma once

#include "JSystem/J3DGraphAnimator/J3DJointTree.h"
#include "JSystem/J3DGraphAnimator/J3DMaterialAttach.h"
#include "JSystem/J3DGraphBase/J3DVertex.h"

class JUTNameTab;

class J3DModelData {
public:
    J3DModelData();

    virtual ~J3DModelData();

    void newSharedDisplayList(u32);
    void clear();

    u32 _4;
    u32 _8;
    u32 _C;
    J3DJointTree mJointTree;            // _10
    J3DMaterialTable mMaterialTable;    // _58
    u32 _78;
    u16 _7C;
    u32 _80;
    u32 _84;
    J3DVertexData mVertexData;          // _88
};