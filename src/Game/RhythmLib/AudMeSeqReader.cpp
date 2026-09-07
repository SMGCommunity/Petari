#include "Game/RhythmLib/AudMeSeqReader.hpp"

void AudMeSeqReader::init(void* pBuff) {
    // FIXME: this function does not match specifically if
    // it appears before call(u32).

    mSeqBuff = (u8*)pBuff;
    mSeqCursor = (u8*)pBuff;
    mNumStacks = 0;

    for (u32 i = 0; i < 8; i++) {
        mStackPtrs[i] = nullptr;
        mLoopCounts[i] = 0;
    }
}

bool AudMeSeqReader::call(u32 addr) {
    if (mNumStacks >= 8) {
        return false;
    }

    mStackPtrs[mNumStacks++] = mSeqCursor;
    mSeqCursor = getAddr(addr);
    return true;
}

bool AudMeSeqReader::ret() {
    if (mNumStacks == 0) {
        return false;
    }

    mSeqCursor = mStackPtrs[--mNumStacks];
    return true;
}

bool AudMeSeqReader::loopStart(u16 loopCount) {
    if (mNumStacks >= 8) {
        return false;
    }

    mStackPtrs[mNumStacks] = mSeqCursor;
    mLoopCounts[mNumStacks++] = loopCount;
    return true;
}

bool AudMeSeqReader::loopEnd() {
    if (mNumStacks == 0) {
        return false;
    }

    u16 loopCount = mLoopCounts[mNumStacks - 1];
    if (loopCount > 0) {
        loopCount--;
    }
    if (loopCount == 0) {
        mNumStacks--;
        return true;
    }
    mLoopCounts[mNumStacks - 1] = loopCount;
    mSeqCursor = mStackPtrs[mNumStacks - 1];
    return true;
}
