#pragma once

#include <revolution.h>

class J3DDrawMtxData {
public:
    J3DDrawMtxData();

    ~J3DDrawMtxData();

    u16 mDrawMatrixCount;  // 0x0
    u16 mRigidMtxCount;    // 0x2
    u32 _4;
    u16* mDrawMtxArray;  // 0x8
};

class J3DVertexData {
public:
    J3DVertexData();
    ~J3DVertexData() {}

    void* getVtxPosArray() const { return mVtxPosArray; }
    void* getVtxNrmArray() const { return mVtxNrmArray; }
    GXColor* getVtxColorArray(u8 idx) const { return mVtxColorArray[idx]; }
    void* getVtxTexCoordArray(u8 idx) const { return mVtxTexCoordArray[idx]; }
    void* getVtxNBTArray() const { return mVtxNBTArray; }
    u32 getNrmNum() const { return mNrmNum; }
    u32 getVtxNum() const { return mVtxNum; }
    GXVtxAttrFmtList* getVtxAttrFmtList() const { return mVtxAttrFmtList; }
    u8 getVtxPosFrac() const { return mVtxPosFrac; }
    u8 getVtxNrmFrac() const { return mVtxNrmFrac; }
    int getVtxPosType() const { return mVtxPosType; }
    int getVtxNrmType() const { return mVtxNrmType; }

    void setVtxPosFrac(u8 frac) { mVtxPosFrac = frac; }
    void setVtxPosType(GXCompType type) { mVtxPosType = type; }
    void setVtxNrmFrac(u8 frac) { mVtxNrmFrac = frac; }
    void setVtxNrmType(GXCompType type) { mVtxNrmType = type; }

private:
    friend class J3DModelLoader;

    /* 0x00 */ u32 mVtxNum;
    /* 0x04 */ u32 mNrmNum;
    /* 0x08 */ u32 mColNum;
    /* 0x0C */ u32 mTexCoordNum;
    /* 0x10 */ u32 mPacketNum;
    /* 0x14 */ GXVtxAttrFmtList* mVtxAttrFmtList;
    /* 0x18 */ void* mVtxPosArray;
    /* 0x1C */ void* mVtxNrmArray;
    /* 0x20 */ void* mVtxNBTArray;
    /* 0x24 */ GXColor* mVtxColorArray[2];
    /* 0x2C */ void* mVtxTexCoordArray[8];
    /* 0x4C */ u8 mVtxPosFrac;
    /* 0x50 */ GXCompType mVtxPosType;
    /* 0x54 */ u8 mVtxNrmFrac;
    /* 0x58 */ GXCompType mVtxNrmType;
};

class J3DVertexBuffer {
public:
    ~J3DVertexBuffer();

    void init();

    u32 _0;
    u32 _4;
    u32 _8;
    u32 _C;
    u32 _10;
    u32 _14;
    u32 _18;
    u32 _1C;
    u32 _20;
    u32 _24;
    u32 _28;
    u32 _2C;
    u32 _30;
    u32 _34;
};
