#pragma once

#include <revolution.h>
#include "JSystem/J3DGraphBase/J3DVertex.h"
#include "JSystem/J3DGraphBase/J3DMatPacket.h"

class J3DModelData;

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

    J3DModelData* mModelData;       // _4
    u32 _8;
    u32 _C;
    u32 _10;
    u32 _14;
    f32 _18;
    f32 _1C;
    f32 _20;
    Mtx _24;
    Mtx _54;
    u32 _84;
    J3DVertexBuffer mVertexBuffer;  // _88
    J3DMatPacket* mMaterialPacket;  // _C0
    u32 _C4;
    u32 _C8;
    u32 _CC;
    u32 _D0;
    u32 _D4;
    u32 _D8;
};