#pragma once

#include "JSystem/JKernel/JKRAramHeap.hpp"
#include "JSystem/JKernel/JKRDvdRipper.hpp"
#include "JSystem/JKernel/JKRThread.hpp"
#include <revolution.h>

u32 JKRDecompExpandSize(u8*);

class JKRAMCommand;

class JKRAram : public JKRThread {
public:
    JKRAram(u32, u32, s32);

    virtual ~JKRAram();
    void* run(void);

    static JKRAram* create(u32, u32, s32, s32, s32);
    static void checkOkAddress(u8*, u32, JKRAramBlock*, u32);
    static void changeGroupIdIfNeed(u8*, int);
    static JKRAramBlock* mainRamToAram(u8*, u32, u32, JKRExpandSwitch, u32, JKRHeap*, int, u32*);
    static u8* aramToMainRam(u32, u8*, u32, JKRExpandSwitch, u32, JKRHeap*, int, u32*);

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

    static JKRAramHeap* getAramHeap() {
        return sAramObject->mAramHeap;
    }

    static u8 decideAramGroupId(int groupId) {
        u8 finalGroupId;
        if (groupId < 0) {
            finalGroupId = getAramHeap()->getCurrentGroupID();
        } else {
            finalGroupId = groupId;
        }
        return finalGroupId;
    }

    static u32 getSZSBufferSize() {
        return sSZSBufferSize;
    }

    static JKRAram* sAramObject;
    static u32 sSZSBufferSize;
    static OSMessageQueue sMessageQueue;
    static OSMessage sMessageBuffer[4];
    static JSUList< JKRAMCommand > sAramCommandList;

    /* 0x7C */ u32 mAudioMemoryPtr;
    /* 0x80 */ u32 mAudioMemorySize;
    /* 0x84 */ u32 mGraphMemoryPtr;
    /* 0x88 */ u32 mGraphMemorySize;
    /* 0x8C */ u32 mAramMemoryPtr;
    /* 0x90 */ u32 mAramMemorySize;
    /* 0x94 */ JKRAramHeap* mAramHeap;
    /* 0x98 */ u32 mStackArray[3];
};

inline JKRAramBlock* JKRAllocFromAram(u32 size, JKRAramHeap::EAllocMode allocMode) {
    return JKRAram::getAramHeap()->alloc(size, allocMode);
}

inline void JKRFreeToAram(JKRAramBlock* block) {
    JKRAram::getAramHeap()->free(block);
}
