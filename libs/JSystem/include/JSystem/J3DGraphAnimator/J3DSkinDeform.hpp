#pragma once

#include "JSystem/J3DGraphAnimator/J3DCluster.hpp"
#include "JSystem/J3DGraphAnimator/J3DMtxBuffer.hpp"

class J3DModel;
class J3DAnmCluster;
class J3DClusterVertex;
class JUTNameTab;
class J3DModelData;

extern void J3DPSMtx33CopyFrom34(__REGISTER MtxPtr src, __REGISTER Mtx3P dst);

struct J3DSkinNList {
    J3DSkinNList();
    void calcSkin_VtxPosF32(f32 (*)[4], void*, void*);
    void calcSkin_VtxNrmF32(f32 (*)[4], void*, void*);

    /* 0x00 */ u16* field_0x0;
    /* 0x04 */ u16* field_0x4;
    /* 0x08 */ f32* field_0x8;
    /* 0x0C */ f32* field_0xc;
    /* 0x10 */ u16 field_0x10;
    /* 0x12 */ u16 field_0x12;
};  // Size: 0x14

class J3DSkinDeform {
public:
    J3DSkinDeform();
    void initSkinInfo(J3DModelData*);
    int initMtxIndexArray(J3DModelData*);
    void changeFastSkinDL(J3DModelData*);
    void calcNrmMtx(J3DMtxBuffer*);
    void transformVtxPosNrm(J3DModelData*);
    void calcAnmInvJointMtx(J3DMtxBuffer*);
    void deformFastVtxPos_F32(J3DVertexBuffer*, J3DMtxBuffer*) const;
    void deformFastVtxNrm_F32(J3DVertexBuffer*, J3DMtxBuffer*) const;
    void deformVtxPos_F32(J3DVertexBuffer*, J3DMtxBuffer*) const;
    void deformVtxPos_S16(J3DVertexBuffer*, J3DMtxBuffer*) const;
    void deformVtxNrm_F32(J3DVertexBuffer*) const;
    void deformVtxNrm_S16(J3DVertexBuffer*) const;
    void deform(J3DModel*);
    void setNrmMtx(int i, MtxPtr mtx) { J3DPSMtx33CopyFrom34(mtx, (Mtx3P)mNrmMtx[i]); }
    Mtx3P getNrmMtx(int i) { return mNrmMtx[i]; }
    void onFlag(u32 flag) { mFlags |= flag; }
    void offFlag(u32 flag) { mFlags &= ~flag; }
    bool checkFlag(u32 flag) { return mFlags & flag ? true : false; }

    virtual void deform(J3DVertexBuffer*, J3DMtxBuffer*);
    virtual ~J3DSkinDeform();

    static u16* sWorkArea_WEvlpMixMtx[1024];
    static f32* sWorkArea_WEvlpMixWeight[1024];
    static u16 sWorkArea_MtxReg[1024];

private:
    /* 0x04 */ u16* mPosData;
    /* 0x08 */ u16* mNrmData;
    /* 0x0C */ Mtx* mPosMtx;
    /* 0x10 */ Mtx33* mNrmMtx;
    /* 0x14 */ u32 mFlags;
    /* 0x18 */ u8 field_0x18;
    /* 0x19 */ u8 field_0x19;
    /* 0x1C */ int field_0x1c;
    /* 0x20 */ int field_0x20;
    /* 0x24 */ J3DSkinNList* mSkinNList;
};

class J3DDeformer {
public:
    J3DDeformer(J3DDeformData*);
    void deform(J3DVertexBuffer*, u16, f32*);
    void deform(J3DVertexBuffer*, u16);
    void deform_VtxPosF32(J3DVertexBuffer*, J3DCluster*, J3DClusterKey*, f32*);
    void deform_VtxNrmF32(J3DVertexBuffer*, J3DCluster*, J3DClusterKey*, f32*);
    void normalizeWeight(int, f32*);

    void offFlag(u32 i_flag) { mFlags &= ~i_flag; }
    bool checkFlag(u32 i_flag) { return mFlags & i_flag ? true : false; }
    void setAnmCluster(J3DAnmCluster* anm) { mAnmCluster = anm; }
    void normalize(f32* i_vec) { PSVECNormalize((Vec*)i_vec, (Vec*)i_vec); }

    /* 0x00 */ J3DDeformData* mDeformData;
    /* 0x04 */ J3DAnmCluster* mAnmCluster;
    /* 0x08 */ f32* field_0x8;
    /* 0x0C */ f32* field_0xc;
    /* 0x10 */ u32 mFlags;
};  // Size: 0x14
