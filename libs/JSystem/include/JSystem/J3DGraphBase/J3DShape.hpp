#pragma once

#include <revolution.h>

class J3DMaterial;
class J3DShapeMtx;
class J3DShapeDraw;
class J3DVertexData;
class J3DDrawMtxData;

class J3DCurrentMtxInfo {
public:
    u32 mMtxIdxRegA; // 0x00
    u32 mMtxIdxRegB; // 0x04
};

class J3DCurrentMtx : public J3DCurrentMtxInfo {
public:
};

class J3DShape {
public:
    inline J3DShape() {
        initialize();
    }

    virtual void draw() const;
    virtual void drawFast() const;
    virtual void simpleDraw() const;
    virtual void simpleDrawCache() const;

    void initialize();

    J3DMaterial*    mMaterial;       // 0x04
    u16             mIndex;          // 0x08
    u16             mMtxGroupNum;    // 0x0A
    u32             mFlags;          // 0x0C
    f32             mRadius;         // 0x10
    Vec             mMin;            // 0x14
    Vec             mMax;            // 0x20
    u8*             mVcdVatCmd;      // 0x2C
    u32*            mVtxDesc;        // 0x30
    bool            mHasNBT;         // 0x34
    J3DShapeMtx**   mShapeMtx;       // 0x38
    J3DShapeDraw**  mShapeDraw;      // 0x3C
    J3DCurrentMtx   mCurrentMtx;     // 0x40
    bool            mHasPNMTXIdx;    // 0x48
    J3DVertexData*  mVertexData;     // 0x4C
    J3DDrawMtxData* mDrawMtxData;    // 0x50
    u8*             mScaleFlagArray; // 0x54
    Mtx**           mDrawMtx;        // 0x58
    Mtx33**         mNormalMtx;      // 0x5C
    u32*            mCurrentViewNo;  // 0x60
    u32             mBumpMtxOffset;  // 0x64

    static u32 sOldVcdVatCmd;
};
