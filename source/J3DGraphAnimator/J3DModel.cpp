#include "J3DGraphAnimator/J3DModel.h"

void J3DModel::initialize()
{
    _4 = 0;
    _8 = 0;
    _C = 0;
    _10 = 0;
    _14 = 0;
    mBaseScale.x = 1.0f;
    mBaseScale.y = 1.0f;
    mBaseScale.z = 1.0f;
    PSMTXIdentity(_24);
    PSMTXIdentity(_54);
    mBuffer = 0;
    _C0 = 0;
    _C4 = 0; 
    _C8 = 0;
    _CC = 0;
    _D0 = 0;
    _D4 = 0;
    _D8 = 0;
}

u8 J3DModel::entryModelData(J3DModelData *pModelData, u32 unk1, u32 unk2)
{
    J3DMtxBuffer* buf = new J3DMtxBuffer();
    buf->initialize();
    mBuffer = buf;

    u8 res = createShapePacket(pModelData);

    if (res != 0)
        return res;
        
    res = createMatPacket(pModelData, unk1);

    if (res != 0)
        return res;
    
    mVertexBuffer.setVertexData(&pModelData->mVertexData);
    prepareShapePackets();

    if (unk1 & 0x1)
        _8 |= 0x1;

    if (unk1 & 0x2)
        _8 |= 0x2;

    if (unk1 & 0x10)
        _8 |= 0x10;

    if (unk1 & 0x20000)
        _8 |= 0x20000;

    if (unk1 & 0x40000)
        _8 |= 0x40000;

    if (unk1 & 0x80000)
        _8 |= 0x80000;

    if (unk1 & 0x4)
        _8 |= 0x4;

    if (unk1 & 0x8)
        _8 |= 0x8;
    
    return 0;
}

u8 J3DModel::createShapePacket(J3DModelData *pModelData)
{
    return 0;
}