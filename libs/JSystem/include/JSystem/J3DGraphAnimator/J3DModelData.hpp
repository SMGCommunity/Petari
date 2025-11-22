#pragma once

#include "JSystem/J3DGraphAnimator/J3DJointTree.hpp"
#include "JSystem/J3DGraphAnimator/J3DMaterialAttach.hpp"
#include "JSystem/J3DGraphBase/J3DVertex.hpp"

class JUTNameTab;

class J3DModelData {
public:
    J3DModelData();

    virtual ~J3DModelData();

    void newSharedDisplayList(u32);
    void clear();

    inline u16 getMaterialCount() { return mMaterialTable.getMaterialCount(); }

    inline J3DMaterial* getMaterial(u16 idx) { return mMaterialTable.mMaterials[idx]; }

    u32 _4;
    u32 _8;
    u32 _C;
    J3DJointTree mJointTree;          // 0x10
    J3DMaterialTable mMaterialTable;  // 0x58
    u32 _78;
    u16 _7C;
    u32 _80;
    u32 _84;
    J3DVertexData mVertexData;  // 0x88
};