#pragma once

#include "JSystem/J3DGraphAnimator/J3DSkinDeform.hpp"
#include "JSystem/J3DGraphBase/J3DPacket.hpp"
#include "JSystem/J3DGraphBase/J3DVertex.hpp"
#include <revolution.h>

class J3DModelData;
class J3DMtxBuffer;
class J3DShapePacket;

enum J3DMdlFlag {
    J3DMdlFlag_None = 0x0,
    /* 0x00001 */ J3DMdlFlag_Unk1 = 0x1,
    /* 0x00002 */ J3DMdlFlag_UseDefaultJ3D = 0x2,
    /* 0x00004 */ J3DMdlFlag_SkinPosCpu = 0x4,
    /* 0x00008 */ J3DMdlFlag_SkinNrmCpu = 0x8,
    /* 0x00010 */ J3DMdlFlag_EnableLOD = 0x10,
    /* 0x20000 */ J3DMdlFlag_UseSharedDL = 0x20000,
    /* 0x40000 */ J3DMdlFlag_UseSingleDL = 0x40000,
    /* 0x80000 */ J3DMdlFlag_DifferedDLBuffer = 0x80000,
};

struct J3DUnkCalc1 {
    virtual void calc(J3DModel* model);
};

struct J3DUnkCalc2 {
    virtual void unk();
    virtual void calc(J3DModelData* mpModelData);
};

typedef void (*J3DCalcCallBack)(J3DModel*, u32 timing);

class J3DModel {
public:
    virtual void update();
    virtual void entry();
    virtual void calc();
    virtual void calcMaterial();
    virtual void calcDiffTexMtx();
    virtual void viewCalc();
    virtual ~J3DModel();

    void setBaseScale(const Vec&);

    J3DModelData* getModelData() { return mModelData; }

    J3DVertexBuffer* getVertexBuffer() { return (J3DVertexBuffer*)&mVertexBuffer; }

    bool checkFlag(u32 flag) const { return (mFlags & flag) ? true : false; }

    J3DMtxBuffer* getMtxBuffer() { return mMtxBuffer; }

    MtxPtr getWeightAnmMtx(int i) { return mMtxBuffer->getWeightAnmMtx(i); }

    /* 0x04 */ J3DModelData* mModelData;
    /* 0x08 */ u32 mFlags;
    /* 0x0C */ u32 mDiffFlag;
    /* 0x10 */ J3DCalcCallBack mCalcCallBack;
    /* 0x14 */ uintptr_t mUserArea;
    /* 0x18 */ Vec mBaseScale;
    /* 0x24 */ Mtx mBaseTransformMtx;
    /* 0x54 */ Mtx mInternalView;
    /* 0x84 */ J3DMtxBuffer* mMtxBuffer;
    /* 0x88 */ J3DVertexBuffer mVertexBuffer;
    /* 0xC0 */ J3DMatPacket* mMatPacket;
    /* 0xC4 */ J3DShapePacket* mShapePacket;
    /* 0xC8 */ J3DDeformData* mDeformData;
    /* 0xCC */ J3DSkinDeform* mSkinDeform;
    /* 0xD0 */ J3DVtxColorCalc* mVtxColorCalc;
    /* 0xD4 */ J3DUnkCalc1* mUnkCalc1;
    /* 0xD8 */ J3DUnkCalc2* mUnkCalc2;
};
