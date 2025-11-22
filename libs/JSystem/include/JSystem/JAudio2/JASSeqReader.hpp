#pragma once

#include <revolution/types.h>

class JASSeqReader {
public:
    void init();
    void init(void*);
    bool call(u32);
    bool loopStart(u32);
    bool loopEnd();
    bool ret();
    u32 readMidiValue();
    void* getStackPtr(u32) const;

    void* mSeqBuff;
    void* mSeqCursor;
    u32 mNumStacks;
    void* mStackPtrs[8];
    u16 mLoopTimers[8];
};