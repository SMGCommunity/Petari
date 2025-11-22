#pragma once

#include <revolution.h>

class JKRDvdFile;
class JKRHeap;

enum JKRExpandSwitch { JKR_EXPAND_SWITCH_1 = 1 };

class JKRDvdRipper {
public:
    enum EAllocDirection { ALLOC_DIRECTION_1 = 1, ALLOC_DIRECTION_2 = 2 };

    static u8* loadToMainRAM(const char*, unsigned char*, JKRExpandSwitch, unsigned long, JKRHeap*, EAllocDirection, unsigned long, int*,
                             unsigned long*);
    static u8* loadToMainRAM(long, unsigned char*, JKRExpandSwitch, unsigned long, JKRHeap*, EAllocDirection, unsigned long, int*, unsigned long*);
    static u8* loadToMainRAM(JKRDvdFile*, unsigned char*, JKRExpandSwitch, unsigned long, JKRHeap*, EAllocDirection, unsigned long, int*,
                             unsigned long*);

    static void isErrorRetry();
};