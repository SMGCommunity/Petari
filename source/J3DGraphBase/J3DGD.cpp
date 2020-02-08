#include "J3DGraphBase/J3DGD.h"
#include "revolution/GD/GDBase.h"
 
const u8 commandTokens_TexImgPtrWrite[7] = { 0x94, 0x95, 0x96, 0xB4, 0xB5, 0xB6, 0xB7 };

void J3DGDSetTexImgPtrRaw(GXTexMapID texMapID, u32 imageAddr)
{
    GDOverflowCheck(5);
    // we create the command by taking the image address and then inserting our command
    J3DGDWriteBPCmd(imageAddr | commandTokens_TexImgPtrWrite[texMapID] << 24);
}

void J3DGDWriteBPCmd(u32 register_val)
{
    *__GDCurrentDL->ptr++ = 0x61;
    J3DGDWrite_u32(register_val);
}

void J3DGDWrite_u32(u32 val)
{
    *__GDCurrentDL->ptr++ = (val >> 24) & 0xFF;
    *__GDCurrentDL->ptr++ = (val >> 16) & 0xFF;
    *__GDCurrentDL->ptr++ = (val >> 8) & 0xFF;
    *__GDCurrentDL->ptr++ = val & 0xFF;
}

void J3DGDWriteXFCmdHdr(u16 addr, u8 length)
{
    u16 val = length - 1;
    *__GDCurrentDL->ptr++ = 0x10;
    J3DGDWrite_u16(val);
    J3DGDWrite_u16(addr);
}

void J3DGDWrite_u16(u16 val)
{
    *__GDCurrentDL->ptr++ = (val >> 8) & 0xFF;
    *__GDCurrentDL->ptr++ = val;
}

void J3DGDWriteCPCmd(u8 addr, u32 val)
{
    *__GDCurrentDL->ptr++ = 8;
    *__GDCurrentDL->ptr++ = addr;
    J3DGDWrite_u32(val);
}