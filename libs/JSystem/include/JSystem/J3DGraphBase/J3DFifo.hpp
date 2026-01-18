#pragma once

#include <revolution/gd.h>
#include <revolution/gx/GXVert.h>

inline void J3DFifoLoadBPCmd(u32 regval) {
    GXCmd1u8(0x61);
    GXCmd1u32(regval);
}

inline void J3DFifoWriteXFCmdHdr(u16 addr, u8 len) {
    GXCmd1u8(0x10);
    GXCmd1u16(len - 1);
    GXCmd1u16(addr);
}
