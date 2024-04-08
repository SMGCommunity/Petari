#ifndef PPC_MEM_H
#define PPC_MEM_H

#include "TRK_Types.h"

static u8 ppc_readbyte1(u8 *addr) {
    u32 *byte = (u32 *)((u32)addr & ~3);

    return (u8)(*byte >> ((3 - ((u32)addr - (u32)byte)) << 3));
}

static void ppc_writebyte1(u8 *addr, u8 byte) {
    u32 *dest = (u32 *)((u32)addr & ~3);
    u32 val = *dest;
    u32 uVar3 = (u32)0xff << ((3 - ((u32)addr - (u32)dest)) << 3);
    u32 iVar1 = (3 - ((u32)addr - (u32)dest)) << 3;

    *dest = (val & ~uVar3) | (uVar3 & (byte << iVar1));
}

#endif // PPC_MEM_H