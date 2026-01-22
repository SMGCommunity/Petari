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

    s32 newSharedDisplayList(u32);
    void indexToPtr();
    void clear();
    void syncJ3DSysFlags() const;

    J3DTexture* getTexture() const { return mMaterialTable.getTexture(); }

    u16 getDrawMtxIndex(u16 idx) const { return mJointTree.getDrawMtxIndex(idx); }

    u16 getMaterialNum() const { return mMaterialTable.getMaterialNum(); }

    Mtx& getInvJointMtx(int idx) { return mJointTree.getInvJointMtx(idx); }

    u8 getDrawMtxFlag(u16 idx) const { return mJointTree.getDrawMtxFlag(idx); }

    u16* getWEvlpImportantMtxIndex() const { return mJointTree.getWEvlpImportantMtxIndex(); }

    J3DMaterial* getMaterialNodePointer(u16 idx) const { return mMaterialTable.getMaterialNodePointer(idx); }

    bool checkFlag(u32 flag) const { return (mFlags & flag) ? true : false; }

    /* 0x04 */ void const* mpRawData;
    /* 0x08 */ u32 mFlags;
    /* 0x0C */ u16 mbHasBumpArray;
    /* 0x0E */ u16 mbHasBillboard;
    /* 0x10 */ J3DJointTree mJointTree;
    /* 0x58 */ J3DMaterialTable mMaterialTable;
    /* 0x78 */ J3DShapeTable mShapeTable;
    /* 0x88 */ J3DVertexData mVertexData;
};
