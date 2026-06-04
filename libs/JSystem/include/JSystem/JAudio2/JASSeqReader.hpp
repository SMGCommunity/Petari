#pragma once

#include <revolution/types.h>

class JASSeqReader {
public:
    JASSeqReader() {
        init();
    }
    void init();
    void init(void*);
    bool call(u32);
    bool loopStart(u32);
    bool loopEnd();
    bool ret();
    u32 readMidiValue();
    void* getStackPtr(u32 idx) const;

    void jump(u32 param_1) {
        mSeqCursor = mSeqBuff + param_1;
    }

    void jump(void* param_1) {
        mSeqCursor = (u8*)param_1;
    }

    u32 get24(u32 param_0) const {
        return (*(u32*)(mSeqBuff + param_0 - 1)) & 0xffffff;
    }

    u32* getBase() {
        return (u32*)mSeqBuff;
    }
    void* getAddr(u32 param_0) {
        return mSeqBuff + param_0;
    }
    u8 getByte(u32 param_0) const {
        return *(mSeqBuff + param_0);
    }
    u16 get16(u32 param_0) const {
        return *(u16*)(mSeqBuff + param_0);
    }
    u32 get32(u32 param_0) const {
        return *(u32*)(mSeqBuff + param_0);
    }
    u8* getCur() {
        return mSeqCursor;
    }
    u32 readByte() {
        return *mSeqCursor++;
    }
    u32 read16() {
#ifdef __MWERKS__
        return *((u16*)mSeqCursor)++;
#else
        u16* value = (u16*)mSeqCursor;
        mSeqCursor += 2;
        return *value;
#endif
    }
    u32 read24() {
        mSeqCursor--;
#ifdef __MWERKS__
        return (*((u32*)mSeqCursor)++) & 0x00ffffff;
#else
        u32* value = (u32*)mSeqCursor;
        mSeqCursor += 4;
        return *value & 0x00ffffff;
#endif
    }
    u16 getLoopCount() const {
        if (mNumStacks == 0) {
            return 0;
        }
        return mLoopTimers[mNumStacks - 1];
    }

    /* 0x00 */ u8* mSeqBuff;
    /* 0x04 */ u8* mSeqCursor;
    /* 0x08 */ u32 mNumStacks;
    /* 0x0C */ u16* mStackPtrs[8];
    /* 0x2C */ u16 mLoopTimers[8];
};
