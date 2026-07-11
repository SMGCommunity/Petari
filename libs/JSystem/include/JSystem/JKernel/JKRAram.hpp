#pragma once

#include "JSystem/JKernel/JKRDvdRipper.hpp"
#include <revolution.h>

u32 JKRDecompExpandSize(u8*);

class JKRAram {
public:
    static void create(u32, u32, s32, s32, s32);
    static void aramToMainRam(u32, u8*, u32, JKRExpandSwitch, u32, JKRHeap*, int, u32*);
    static bool mainRamToAram(u8*, u32, u32, JKRExpandSwitch, u32, JKRHeap*, int, u32*);

    static JKRAram* getManager() {
        return sAramObject;
    }

    u32 getAudioMemory() const {
        return mAudioMemoryPtr;
    }
    u32 getAudioMemSize() const {
        return mAudioMemorySize;
    }
    u32 getGraphMemory() const {
        return mGraphMemoryPtr;
    }
    u32 getGraphMemSize() const {
        return mGraphMemorySize;
    }

    static JKRAram* sAramObject;

    /* 0x7C */ u32 mAudioMemoryPtr;
    /* 0x80 */ u32 mAudioMemorySize;
    /* 0x84 */ u32 mGraphMemoryPtr;
    /* 0x88 */ u32 mGraphMemorySize;
    /* 0x8C */ u32 mAramMemoryPtr;
    /* 0x90 */ u32 mAramMemorySize;
    /* 0x94 */ u32 _94;  // JKRAramHeap* mAramHeap;
    /* 0x98 */ u32 mStackArray[3];
};
