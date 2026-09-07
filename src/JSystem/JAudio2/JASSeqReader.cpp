#include "JSystem/JAudio2/JASSeqReader.hpp"

void JASSeqReader::init() {
    mSeqBuff = nullptr;
    mSeqCursor = nullptr;
    mNumStacks = 0;
    for (u32 i = 0; i < 8; i++) {
        mStackPtrs[i] = nullptr;
        mLoopCounts[i] = nullptr;
    }
}

void JASSeqReader::init(void* buf) {
    mSeqBuff = (u8*)buf;
    mSeqCursor = (u8*)buf;
    mNumStacks = 0;
    for (u32 i = 0; i < 8; i++) {
        mStackPtrs[i] = nullptr;
        mLoopCounts[i] = 0;
    }
}

bool JASSeqReader::call(u32 addr) {
    if (mNumStacks >= 8)
        return false;
    else {
        mStackPtrs[mNumStacks++] = mSeqCursor;
        mSeqCursor = getAddr(addr);
        return true;
    }
}

bool JASSeqReader::loopStart(u32 loopCount) {
    if (mNumStacks >= 8)
        return false;
    else {
        mStackPtrs[mNumStacks] = mSeqCursor;
        mLoopCounts[mNumStacks++] = loopCount;
        return true;
    }
}

bool JASSeqReader::loopEnd() {
    if (mNumStacks == 0)
        return false;
    else {
        u16 loopCount = mLoopCounts[mNumStacks - 1];
        if (loopCount > 0)
            loopCount--;
        if (loopCount == 0) {
            mNumStacks--;
            return true;
        }
        mLoopCounts[mNumStacks - 1] = loopCount;
        mSeqCursor = mStackPtrs[mNumStacks - 1];
        return true;
    }
}

bool JASSeqReader::ret() {
    if (mNumStacks == 0)
        return false;
    else {
        mSeqCursor = mStackPtrs[--mNumStacks];
        return true;
    }
}

u32 JASSeqReader::readMidiValue() {
    u8 byte = readByte();
    if (!(byte & 0x80))
        return byte;
    u32 byte2 = byte & 0x7f;
    s32 i = 0;
    u8 byte4;
    while (true) {
        if (i > 2)
            return 0;
        byte2 = byte2 << 7;
        byte4 = readByte();
        byte2 |= byte4 & 0x7f;
        if (!(byte4 & 0x80))
            break;
        i++;
    }
    return byte2;
}

void* JASSeqReader::getStackPtr(u32 idx) const {
    if (idx >= mNumStacks)
        return nullptr;
    else
        return mStackPtrs[idx];
}
