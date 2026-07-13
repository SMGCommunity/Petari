#pragma once

#include <revolution.h>

class JKRDvdFile;
class JKRHeap;

enum JKRExpandSwitch {
    EXPAND_SWITCH_UNKNOWN0 = 0,
    EXPAND_SWITCH_UNKNOWN1 = 1,
    EXPAND_SWITCH_UNKNOWN2 = 2,
};

struct SYaz0Header {
    u32 signature;
    u32 length;
};

class JKRDvdRipper {
public:
    enum EAllocDirection {
        UNKNOWN_EALLOC_DIRECTION = 0,
        ALLOC_DIRECTION_FORWARD = 1,
        ALLOC_DIRECTION_BACKWARD = 2,
    };

    static void* loadToMainRAM(const char*, unsigned char*, JKRExpandSwitch, unsigned long, JKRHeap*, EAllocDirection, unsigned long, int*,
                               unsigned long*);
    static void* loadToMainRAM(long, unsigned char*, JKRExpandSwitch, unsigned long, JKRHeap*, EAllocDirection, unsigned long, int*, unsigned long*);
    static void* loadToMainRAM(JKRDvdFile*, unsigned char*, JKRExpandSwitch, unsigned long, JKRHeap*, EAllocDirection, unsigned long, int*,
                               unsigned long*);

    static bool isErrorRetry();
    inline static u32 getSZSBufferSize() {
        return sSZSBufferSize;
    }

    static bool errorRetry;
    static u32 sSZSBufferSize;
};

inline void* JKRDvdToMainRam(JKRDvdFile* file, u8* dst, JKRExpandSwitch expandSwitch, u32 dstLength, JKRHeap* heap,
                             JKRDvdRipper::EAllocDirection allocDirection, u32 offset, int* compression, u32* returnSize) {
    return JKRDvdRipper::loadToMainRAM(file, dst, expandSwitch, dstLength, heap, allocDirection, offset, compression, returnSize);
}

inline void* JKRDvdToMainRam(s32 entryNum, u8* dst, JKRExpandSwitch expandSwitch, u32 dstLength, JKRHeap* heap,
                             JKRDvdRipper::EAllocDirection allocDirection, u32 offset, int* compression, u32* returnSize) {
    return JKRDvdRipper::loadToMainRAM(entryNum, dst, expandSwitch, dstLength, heap, allocDirection, offset, compression, returnSize);
}

inline void* JKRDvdToMainRam(const char* name, u8* dst, JKRExpandSwitch expandSwitch, u32 dstLength, JKRHeap* heap,
                             JKRDvdRipper::EAllocDirection allocDirection, u32 offset, int* compression, u32* returnSize) {
    return JKRDvdRipper::loadToMainRAM(name, dst, expandSwitch, dstLength, heap, allocDirection, offset, compression, returnSize);
}
