#pragma once

#include <revolution.h>

class J3DModelData;
class J3DMaterialTable;
class J3DModelHierarchy;

struct J3DModelBlock {
    u32 mBlockType;
    u32 mBlockSize;
};

struct J3DModelFileData {
    u32           mMagic1;
    u32           mMagic2;
    u8            _8[4];
    u32           mBlockNum;
    u8            _10[0x10];
    J3DModelBlock mBlocks[1];
};

struct J3DModelInfoBlock : public J3DModelBlock {
    u16   mFlags;
    u32   mPacketNum;
    u32   mVtxNum;
    void* mpHierarchy;
};

struct J3DVertexBlock : public J3DModelBlock {
    void* mpVtxAttrFmtList;
    void* mpVtxPosArray;
    void* mpVtxNrmArray;
    void* mpVtxNBTArray;
    void* mpVtxColorArray[2];
    void* mpVtxTexCoordArray[8];
};

struct J3DEnvelopeBlock : public J3DModelBlock {
    u16   mWEvlpMtxNum;
    void* mpWEvlpMixMtxNum;
    void* mpWEvlpMixIndex;
    void* mpWEvlpMixWeight;
    void* mpInvJointMtx;
};

struct J3DDrawBlock : public J3DModelBlock {
    u16   mMtxNum;
    void* mpDrawMtxFlag;
    void* mpDrawMtxIndex;
};

struct J3DJointBlock : public J3DModelBlock {
    u16   mJointNum;
    void* mpJointInitData;
    void* mpIndexTable;
    void* mpNameTable;
};

struct J3DMaterialBlock : public J3DModelBlock {
    u16   mMaterialNum;
    void* mpMaterialInitData;
    void* mpMaterialID;
    void* mpNameTable;
    void* mpIndInitData;
    void* mpCullMode;
    void* mpMatColor;
    void* mpColorChanNum;
    void* mpColorChanInfo;
    void* mpAmbColor;
    void* mpLightInfo;
    void* mpTexGenNum;
    void* mpTexCoordInfo;
    void* mpTexCoord2Info;
    void* mpTexMtxInfo;
    void* _44;
    void* mpTexNo;
    void* mpTevOrderInfo;
    void* mpTevColor;
    void* mpTevKColor;
    void* mpTevStageNum;
    void* mpTevStageInfo;
    void* mpTevSwapModeInfo;
    void* mpTevSwapModeTableInfo;
    void* mpFogInfo;
    void* mpAlphaCompInfo;
    void* mpBlendInfo;
    void* mpZModeInfo;
    void* mpZCompLoc;
    void* mpDither;
    void* mpNBTScaleInfo;
};

struct J3DMaterialBlock_v21 : public J3DModelBlock {
    u16   mMaterialNum;
    void* mpMaterialInitData;
    void* mpMaterialID;
    void* mpNameTable;
    void* mpCullMode;
    void* mpMatColor;
    void* mpColorChanNum;
    void* mpColorChanInfo;
    void* mpTexGenNum;
    void* mpTexCoordInfo;
    void* mpTexCoord2Info;
    void* mpTexMtxInfo;
    void* _38;
    void* mpTexNo;
    void* mpTevOrderInfo;
    void* mpTevColor;
    void* mpTevKColor;
    void* mpTevStageNum;
    void* mpTevStageInfo;
    void* mpTevSwapModeInfo;
    void* mpTevSwapModeTableInfo;
    void* mpFogInfo;
    void* mpAlphaCompInfo;
    void* mpBlendInfo;
    void* mpZModeInfo;
    void* mpZCompLoc;
    void* mpDither;
    void* mpNBTScaleInfo;
};

struct J3DMaterialDLBlock : public J3DModelBlock {
    u16   mMaterialNum;
    void* mpDisplayListInit;
    void* mpPatchingInfo;
    void* mpCurrentMtxInfo;
    void* mpMaterialMode;
    void* _1C;
    void* mpNameTable;
};

struct J3DShapeBlock : public J3DModelBlock {
    u16   mShapeNum;
    void* mpShapeInitData;
    void* mpIndexTable;
    void* mpNameTable;
    void* mpVtxDescList;
    void* mpMtxTable;
    void* mpDisplayListData;
    void* mpMtxInitData;
    void* mpDrawInitData;
};

struct J3DTextureBlock : public J3DModelBlock {
    u16   mTextureNum;
    void* mpTextureRes;
    void* mpNameTable;
};

class J3DModelLoader {
public:
    J3DModelLoader();

    virtual J3DModelData*     load(const void*, u32);
    virtual J3DMaterialTable* loadMaterialTable(const void*);
    virtual J3DModelData*     loadBinaryDisplayList(const void*, u32);
    virtual u32               calcLoadSize(const void*, u32);
    virtual u32               calcLoadMaterialTableSize(const void*);
    virtual u32               calcLoadBinaryDisplayListSize(void const*, u32);
    virtual u16               countMaterialNum(void const*);
    virtual void              setupBBoardInfo();
    virtual ~J3DModelLoader() {
    }
    virtual void readMaterial(J3DMaterialBlock const*, u32) {
    }

    virtual void readMaterial_v21(J3DMaterialBlock_v21 const*, u32) {
    }

    virtual void readMaterialTable(J3DMaterialBlock const*, u32) {
    }

    virtual void readMaterialTable_v21(J3DMaterialBlock_v21 const*, u32) {
    }

    virtual u32 calcSizeMaterial(J3DMaterialBlock const*, u32) {
        return false;
    }

    virtual u32 calcSizeMaterialTable(J3DMaterialBlock const*, u32) {
        return false;
    }

    J3DModelData*           mpModelData;      // 0x04
    J3DMaterialTable*       mpMaterialTable;  // 0x08
    const J3DShapeBlock*    mpShapeBlock;     // 0x0C
    const J3DMaterialBlock* mpMaterialBlock;  // 0x10
    J3DModelHierarchy*      mpModelHierarchy; // 0x14
    u8                      _18;
    u8                      _19;
    u16                     mEnvelopeSize; // 0x1A
};

class J3DModelLoader_v21 : public J3DModelLoader {
public:
    inline J3DModelLoader_v21() {
    }

    ~J3DModelLoader_v21() {
    }

    void readMaterial_v21(J3DMaterialBlock_v21 const*, u32);
    void readMaterialTable_v21(J3DMaterialBlock_v21 const*, u32);
};

class J3DModelLoader_v26 : public J3DModelLoader {
public:
    ~J3DModelLoader_v26() {
    }

    void readMaterial(J3DMaterialBlock const*, u32);
    void readMaterialTable(J3DMaterialBlock const*, u32);
    u32  calcSizeMaterial(J3DMaterialBlock const*, u32);
    u32  calcSizeMaterialTable(J3DMaterialBlock const*, u32);
};

class J3DModelLoaderDataBase {
public:
    static J3DModelData* load(void const*, u32);
};
