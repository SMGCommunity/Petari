#include "J3DGraphBase/J3DSys.h"

void J3DSys::loadPosMtxIndx(s32 unk1, u16 unk2) const
{
    s32 val = (unk1 * 0xC) & 0xFFFF;
    *((vu8*)GPU_FIFO_ADDR) = 0x20;
    *((vu16*)GPU_FIFO_ADDR) = unk2;
    *((vs16*)GPU_FIFO_ADDR) = val | 0xB000;
}

void J3DSys::loadNrmMtxIndx(s32 unk1, u16 unk2) const
{
    s32 val = ((unk1 * 0x9) + 0x400) & 0xFFFF;
    *((vu8*)GPU_FIFO_ADDR) = 0x28;
    *((vu16*)GPU_FIFO_ADDR) = unk2;
    *((vs16*)GPU_FIFO_ADDR) = val | 0xB000;
}