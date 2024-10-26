#include "Game/System/DrawSyncManager.hpp"

DrawSyncManager* DrawSyncManager::start(u32 a1, s32 a2) {
    if (!DrawSyncManager::sInstance) {
        DrawSyncManager::sInstance = new DrawSyncManager(a1, a2);
    }

    return DrawSyncManager::sInstance;
}

void DrawSyncManager::prepareReset() {
    if (!DrawSyncManager::sInstance) {
        return;
    }

    DrawSyncManager::sInstance->reset(true);
}

void DrawSyncManager::resetIfAborted() {
    if (!DrawSyncManager::sInstance) {
        return;
    }

    DrawSyncManager::sInstance->reset(false);
}

void DrawSyncManager::clearFifo() {
    while (mFifo->getCount() != 0) {
        mFifo->pop();
    }
}

Fifo* Fifo::pop() {
    Fifo* cur = mArray[mLoopIdx];
    mLoopIdx = getLoopIdx(mLoopIdx + 1);
    return cur;
}

u32 Fifo::getLoopIdx(u32 idx) {
    if (idx >= mCount + 1) {
        idx = 0;
    }

    return idx;
}

u32 Fifo::getCount() {
    if (mLoopIdx < _C) {
        return _C - mLoopIdx;
        
    }

    return _C + mCount + 1 - mLoopIdx;
}

void DrawSyncManager::end() {
    if (!DrawSyncManager::sInstance) {
        return;
    }

    delete DrawSyncManager::sInstance;
    DrawSyncManager::sInstance = 0;
}

DrawSyncManager::~DrawSyncManager() {
    GXSetDrawSyncCallback(0);
    GXDisableBreakPt();
    OSSendMessage(&mQueue, (void*)0x10000, OS_MESSAGE_BLOCK);
    OSJoinThread(&mThread, 0);
}

/*
void DrawSyncManager::reset(bool arg) {
    if (!arg && _372) {
        _372 = 0;
        _373 = 1;

        GXSetDrawSyncCallback(DrawSyncManager::drawSyncCallback);
    }
}
*/

// DrawSyncManager::drawSyncCallbackSub

void DrawSyncManager::pushBreakPoint() {
    if (_36C & 0x3) {
        GXFlush();
        GXFifoObj fifoObj;
        GXGetCPUFifo(&fifoObj);

        void *readPtr, *writePtr;

        GXGetFifoPtrs(&fifoObj, &readPtr, &writePtr);
        OSSendMessage(&mQueue, writePtr, OS_MESSAGE_BLOCK);
    }
}