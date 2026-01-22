#pragma once

#include "JSystem/J3DGraphAnimator/J3DJointTree.hpp"
#include "JSystem/J3DGraphAnimator/J3DMaterialAttach.hpp"
#include "JSystem/J3DGraphAnimator/J3DShapeTable.hpp"
#include "JSystem/J3DGraphBase/J3DVertex.hpp"

class JUTNameTab;

class J3DModelData {
public:
    J3DModelData();

    virtual ~J3DModelData();

    void newSharedDisplayList(u32);
    void clear();
    void syncJ3DSysFlags() const;

    u16 getDrawMtxIndex(u16 idx) const { return mJointTree.getDrawMtxIndex(idx); }

    inline u16 getMaterialCount() { return mMaterialTable.getMaterialCount(); }

    inline J3DMaterial* getMaterial(u16 idx) { return mMaterialTable.mMaterials[idx]; }

    Mtx& getInvJointMtx(int idx) { return mJointTree.getInvJointMtx(idx); }

    u8 getDrawMtxFlag(u16 idx) const { return mJointTree.getDrawMtxFlag(idx); }

    u16* getWEvlpImportantMtxIndex() const { return mJointTree.getWEvlpImportantMtxIndex(); }

    /* 0x04 */ void const* mpRawData;
    /* 0x08 */ u32 mFlags;
    /* 0x0C */ u16 mbHasBumpArray;
    /* 0x0E */ u16 mbHasBillboard;
    /* 0x10 */ J3DJointTree mJointTree;
    /* 0x58 */ J3DMaterialTable mMaterialTable;
    /* 0x78 */ J3DShapeTable mShapeTable;
    /* 0x88 */ J3DVertexData mVertexData;
};
