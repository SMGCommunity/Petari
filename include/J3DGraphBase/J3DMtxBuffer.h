#pragma once

#include "J3DGraphBase/J3DModelData.h"

class J3DMtxBuffer
{
public:
    inline J3DMtxBuffer() { }

    virtual ~J3DMtxBuffer();

    void initialize();
    void create(J3DModelData *, u64);
    void createAnmMtx(J3DModelData *);
    void createWeightEnvelopeMtx(J3DModelData *);
    void setNoUseDrawMtx();
    void createDoubleDrawMtx(J3DModelData *, u64);

    u32 _0;
    u32 _4;
    u32 _8;
    u32 _C;
    Mtx44* _10;
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