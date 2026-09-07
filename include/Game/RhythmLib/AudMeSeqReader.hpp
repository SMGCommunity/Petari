#pragma once

#include "JSystem/JAudio2/JASSeqReader.hpp"

class AudMeSeqReader {
public:
    AudMeSeqReader() {
        init();
    }

    void init() {
        init(nullptr);
    }

    void init(void*);
    bool call(u32);
    bool loopStart(u16);
    bool loopEnd();
    bool ret();

    void jump(u32 addr) {
        mSeqCursor = mSeqBuff + addr;
    }

    void jump(void* pPtr) {
        mSeqCursor = (u8*)pPtr;
    }

    u32* getBase() {
        return (u32*)mSeqBuff;
    }
    u8* getAddr(u32 addr) {
        return mSeqBuff + addr;
    }
    u8 getByte(u32 addr) const {
        return *(mSeqBuff + addr);
    }
    u16 get16(u32 addr) const {
        return *(u16*)(mSeqBuff + addr);
    }
    u32 get24(u32 addr) const {
        return (*(u32*)(mSeqBuff + addr - 1)) & 0xffffff;
    }
    u32 get32(u32 addr) const {
        return *(u32*)(mSeqBuff + addr);
    }
    u8* getCur() {
        return mSeqCursor;
    }
    u32 read8() {
        return *mSeqCursor++;
    }
    u32 read16() {
        u32 a = read8();
        a <<= 8;
        a |= read8();
        // u32 b = read8();
        // return (read8() << 8 | read8());
        return a;
    }
    u32 read24() {
        u32 ret = read8() << 8 | read8();
        ret <<= 8;
        ret |= read8();
        return ret;
    }
    u16 getLoopCount() const {
        if (mNumStacks == 0) {
            return 0;
        }
        return mLoopCounts[mNumStacks - 1];
    }

    /* 0x00 */ u8* mSeqBuff;
    /* 0x04 */ u8* mSeqCursor;
    /* 0x08 */ u32 mNumStacks;
    /* 0x0C */ u8* mStackPtrs[8];
    /* 0x2C */ u16 mLoopCounts[8];
};
