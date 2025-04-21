#pragma once

#include <revolution.h>
#include "JSystem/J3DGraphBase/J3DVertex.hpp"
#include "JSystem/J3DGraphBase/J3DMatPacket.hpp"

class J3DModelData;
class J3DMtxBuffer;
class J3DShapePacket;

class J3DModel {
public:
    virtual void update();
    virtual void entry();
    virtual void calc();
    virtual void calcMaterial();
    virtual void calcDiffTexMtx();
    virtual void viewCalc();
    virtual ~J3DModel();

    void setBaseScale(const Vec &);

    J3DModelData* mModelData;       // 0x4
    u32 _8;
    u32 _C;
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
