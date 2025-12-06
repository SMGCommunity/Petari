#pragma once

#include "JSystem/JKernel/JKRDvdRipper.hpp"
#include <revolution.h>

s32 JKRDecompExpandSize(u8*);

class JKRAram {
public:
    static void create(u32, u32, s32, s32, s32);
    static void aramToMainRam(u32, u8*, u32, JKRExpandSwitch, u32, JKRHeap*, int, u32*);
};
