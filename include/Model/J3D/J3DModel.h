#ifndef J3DMODEL_H
#define J3DMODEL_H

#include "Model/J3D/J3DModelData.h"
#include "os/mtx.h"

class J3DModel
{
public:
    virtual void update();
    virtual void entry();
    virtual void calc();
    virtual void calcMaterial();
    virtual void calcDiffTexMtx();
    virtual void viewCalc();
    virtual ~J3DModel();

    void initialize();
    void entryModelData(J3DModelData *, u64, u64);
    void createShapePacket(J3DModelData *);
    void createMatPacket(J3DModelData *, u64);
    void lock();
    void unlock();
    void makeDL();
    void calcAnmMtx();
    void calcWeightEnvelopeMtx();
    void calcBBoardMtx();
    void prepareShapePackets();
    Mtx33* getDrawMtxPtr();
    void setBaseScale(const Vec &);

    u32 _4;
    u32 _8;
    u32 _C;
    u32 _10;
    u32 _14;
    Vec mBaseScale; // _18
    Mtx _24;
    Mtx _54;
    u32* _84; // J3DMtxBuffer*
    J3DVertexBuffer mVertexBuffer; // _88
    u32* _C0; // J3DMatPacket*
    u32 _C4;
    u32 _C8;
    u32 _CC;
    u32 _D0;
    u32 _D4;
    u32 _D8;
};

const static f32 J3DDefaultScale = 1.0f;

#endif // J3DMODEL_H