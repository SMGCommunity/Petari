#pragma once

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

    J3DModelData* mModelData;  // 0x4
    u32 mFlags;
    u32 mDiffFlag;
    u32 _10;
    u32 _14;
    f32 _18;
    f32 _1C;
    f32 _20;
    Mtx mBaseTransformMtx;          // 0x24
    Mtx mInternalView;              // 0x54
    J3DMtxBuffer* mMtxBuffer;       // 0x84
    J3DVertexBuffer mVertexBuffer;  // 0x88
    J3DMatPacket* mMaterialPacket;  // 0xC0
    u32 _C4;
    u32 _C8;
    u32 _CC;
    u32 _D0;
    u32 _D4;
    u32 _D8;
};
