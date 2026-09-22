#pragma once

#include <revolution.h>

class DrawSyncCallback {
public:
    DrawSyncCallback() {
    }

    virtual void drawSyncCallback(u16 token) = 0;
};

class Fifo {
public:
    Fifo(u32 capacity) : mCount(capacity), mLoopIdx(), mWriteIndex() {
        mArray = new void*[capacity + 1];
    }

    void push(void* pValue) {
        mArray[mWriteIndex] = pValue;
        mWriteIndex = getLoopIdx(mWriteIndex + 1);
    }

    void* pop();
    u32 getLoopIdx(u32 index);
    u32 getCount();

    void* peek(u32 offset) {
        return mArray[getLoopIdx(mLoopIdx + offset)];
    }

    /* 0x00 */ void** mArray;
    /* 0x04 */ u32 mCount;
    /* 0x08 */ u32 mLoopIdx;
    /* 0x0C */ u32 mWriteIndex;
};

class DrawSyncManager {
public:
    struct TDrawSyncTokenRange {
        TDrawSyncTokenRange() : mStart(), mEnd(), mCallback() {
        }

        TDrawSyncTokenRange(u16 start, u16 end, DrawSyncCallback* pCallback) : mStart(start), mEnd(end), mCallback(pCallback) {
        }

        /* 0x00 */ u16 mStart;
        /* 0x02 */ u16 mEnd;
        /* 0x04 */ DrawSyncCallback* mCallback;
    };

    DrawSyncManager(u32 capacity, s32 priority);

    ~DrawSyncManager();

    void reset(bool preparing);

    static DrawSyncManager* start(u32 capacity, s32 priority);
    static void prepareReset();
    static void resetIfAborted();

    void clearFifo();
    static void end();

    void pushBreakPoint();
    void drawSyncCallbackSub(u16 token);

    static void drawSyncCallback(u16 token);
    static void* threadFunc(void* pArg);

    u16 setCallback(u32 index, u16 count, DrawSyncCallback* pCallback);

    static DrawSyncManager* sInstance;

    /* 0x000 */ TDrawSyncTokenRange mTokenRanges[5];
    /* 0x028 */ OSThread mThread;
    /* 0x340 */ OSMessageQueue mQueue;
    /* 0x360 */ u8* mStack;
    /* 0x364 */ OSMessage* mMessageBuffer;
    /* 0x368 */ Fifo* mFifo;
    /* 0x36C */ u16 mFlags;
    /* 0x36E */ u16 mLowTokenCount;
    /* 0x370 */ u16 mHighTokenCount;
    /* 0x372 */ bool mPreparingReset;
    /* 0x373 */ volatile bool mAborted;
    /* 0x374 */ u32 _374;
};
