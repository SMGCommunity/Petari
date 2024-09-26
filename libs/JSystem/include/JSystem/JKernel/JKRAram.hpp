#pragma once

#include "JSystem/JKernel/JKRDvdRipper.hpp"
#include <revolution.h>

s32 JKRDecompExpandSize(unsigned char *);

class JKRAram {
public:
    static void aramToMainRam(unsigned long, unsigned char *, unsigned long, JKRExpandSwitch, unsigned long, JKRHeap *, int, unsigned long *);
};
