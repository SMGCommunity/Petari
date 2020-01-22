#include "J3DGraphBase/J3DGD.h"
#include "OS/GD/GDBase.h"

void J3DGDWriteBPCmd(u32 unk1)
{
    *__GDCurrentDL->mPtr++ = 0x61;
    J3DGDWrite_u32(unk1);
}

void J3DGDWrite_u32(u32 val)
{
    *__GDCurrentDL->mPtr++ = (val >> 24) & 0xFF;
    *__GDCurrentDL->mPtr++ = (val >> 16) & 0xFF;
    *__GDCurrentDL->mPtr++ = (val >> 8) & 0xFF;
    *__GDCurrentDL->mPtr++ = val & 0xFF;
}

void J3DGDWriteXFCmdHdr(u16 unk1, u8 unk2)
{
    u16 val = unk2 - 1;
    *__GDCurrentDL->mPtr++ = 0x10;
    J3DGDWrite_u16(val);
    J3DGDWrite_u16(unk1);
}

void J3DGDWrite_u16(u16 val)
{
    *__GDCurrentDL->mPtr++ = (val >> 8) & 0xFF;
    *__GDCurrentDL->mPtr++ = val;
}

void J3DGDWriteCPCmd(u8 unk1, u32 unk2)
{
    *__GDCurrentDL->mPtr++ = 8;
    *__GDCurrentDL->mPtr++ = unk1;
    J3DGDWrite_u32(unk2);
}