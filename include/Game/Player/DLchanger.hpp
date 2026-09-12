#pragma once

#include <JSystem/JKernel/JKRHeap.hpp>
#include <revolution/types.h>

class J3DModelX;

struct DLholder {
    /* 0x00 */ u8* mDL;
    /* 0x04 */ u16 mSize;
    /* 0x06 */ u16 _6;
};

class DLchanger {
public:
    DLchanger(u8 count, u32 size) {
        mBuffers = new DLholder[count];
        mBufferCount = count;
        mCurrentBuffer = 0;
        for (u32 i = 0; i < mBufferCount; i++) {
            mBuffers[i].mDL = new (32) u8[size];
        }
    }

    void addDL(J3DModelX*);

    DLholder* swap() {
        mCurrentBuffer = (mCurrentBuffer + 1) % mBufferCount;
        return &mBuffers[mCurrentBuffer];
    }

    /* 0x00 */ DLholder* mBuffers;
    /* 0x04 */ u8 mBufferCount;
    /* 0x05 */ u8 mCurrentBuffer;
};
