#include "Game/System/DrawSyncManager.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/Util/SingletonHolder.hpp"

DrawSyncManager* DrawSyncManager::sInstance;

DrawSyncManager* DrawSyncManager::start(u32 capacity, s32 priority) {
    if (sInstance == nullptr) {
        sInstance = new DrawSyncManager(capacity, priority);
    }

    return sInstance;
}

void DrawSyncManager::prepareReset() {
    if (sInstance == nullptr) {
        return;
    }

    sInstance->reset(true);
}

void DrawSyncManager::resetIfAborted() {
    if (sInstance == nullptr) {
        return;
    }

    sInstance->reset(false);
}

void DrawSyncManager::clearFifo() {
    while (mFifo->getCount() != 0) {
        mFifo->pop();
    }
}

void* Fifo::pop() {
    void* pValue = mArray[mLoopIdx];
    mLoopIdx = getLoopIdx(mLoopIdx + 1);
    return pValue;
}

u32 Fifo::getLoopIdx(u32 index) {
    if (index >= mCount + 1) {
        index = 0;
    }

    return index;
}

u32 Fifo::getCount() {
    if (mLoopIdx <= mWriteIndex) {
        return mWriteIndex - mLoopIdx;
    }

    return mWriteIndex + mCount + 1 - mLoopIdx;
}

void DrawSyncManager::end() {
    if (sInstance == nullptr) {
        return;
    }

    delete sInstance;
    sInstance = nullptr;
}

void DrawSyncManager::drawSyncCallback(u16 token) {
    if (sInstance != nullptr) {
        sInstance->drawSyncCallbackSub(token);
    }
}

void* DrawSyncManager::threadFunc(void* pArg) {
    DrawSyncManager* pManager = static_cast< DrawSyncManager* >(pArg);

    while (true) {
        OSMessage message;
        OSReceiveMessage(&pManager->mQueue, &message, OS_MESSAGE_BLOCK);

        if (reinterpret_cast< u32 >(message) >= 0x80000000) {
            pManager->mFifo->push(message);
            const bool aborted = pManager->mAborted;

            if (pManager->mFifo->getCount() == 2) {
                GXEnableBreakPt(message);
            }
        } else if (reinterpret_cast< u32 >(message) < 0x10000) {
            pManager->mFifo->pop();
            const bool aborted = pManager->mAborted;

            u32 count = pManager->mFifo->getCount();
            if (count != 0) {
                if (count == 1) {
                    GXDisableBreakPt();
                } else if (count >= 2) {
                    GXEnableBreakPt(pManager->mFifo->peek(1));
                }
            }
        } else {
            break;
        }
    }

    return nullptr;
}

DrawSyncManager::DrawSyncManager(u32 capacity, s32 priority) {
    mPreparingReset = false;
    mStack = nullptr;
    mMessageBuffer = nullptr;
    mFlags = 0;
    mLowTokenCount = 0;
    mHighTokenCount = 0;
    mAborted = false;

    mStack = new u8[0x8000];
    OSCreateThread(&mThread, threadFunc, this, mStack + 0x8000, 0x8000, priority, 0);
    mMessageBuffer = new OSMessage[20];
    OSInitMessageQueue(&mQueue, mMessageBuffer, 20);
    mFifo = new Fifo(capacity);
    OSResumeThread(&mThread);
    GXSetDrawSyncCallback(drawSyncCallback);
}

DrawSyncManager::~DrawSyncManager() {
    GXSetDrawSyncCallback(nullptr);
    GXDisableBreakPt();
    OSSendMessage(&mQueue, reinterpret_cast< OSMessage >(0x10000), OS_MESSAGE_BLOCK);
    OSJoinThread(&mThread, nullptr);
}

u16 DrawSyncManager::setCallback(u32 index, u16 count, DrawSyncCallback* pCallback) {
    if (index < 3) {
        u16 start = mLowTokenCount + 1;
        mTokenRanges[index] = TDrawSyncTokenRange(start, start + count - 1, pCallback);
        mLowTokenCount += count;
        return start;
    }

    u16 start = mHighTokenCount + 0xA000;
    mTokenRanges[index] = TDrawSyncTokenRange(start, start + count - 1, pCallback);
    mHighTokenCount += count;
    return start;
}

void DrawSyncManager::reset(bool preparing) {
    if (preparing) {
        mPreparingReset = true;
        return;
    }

    if (mPreparingReset) {
        mPreparingReset = false;
        mAborted = true;
        GXSetDrawSyncCallback(drawSyncCallback);
        SingletonHolder< GameSystem >::get()->initGX();
    }
}

void DrawSyncManager::drawSyncCallbackSub(u16 token) {
    if (token == 0) {
        if ((mFlags & 2) == 0) {
            OSSendMessage(&mQueue, reinterpret_cast< OSMessage >(token), OS_MESSAGE_BLOCK);
        }

        return;
    }

    for (u16 i = 0; i < 5; i++) {
        if (mTokenRanges[i].mCallback != nullptr && mTokenRanges[i].mStart <= token && token <= mTokenRanges[i].mEnd) {
            mTokenRanges[i].mCallback->drawSyncCallback(token);

            if ((mFlags & 2) == 0) {
                OSSendMessage(&mQueue, reinterpret_cast< OSMessage >(token), OS_MESSAGE_BLOCK);
            }

            break;
        }
    }
}

void DrawSyncManager::pushBreakPoint() {
    if ((mFlags & 3) == 0) {
        GXFlush();
        GXFifoObj fifoObj;
        GXGetCPUFifo(&fifoObj);

        void* pRead;
        void* pWrite;
        GXGetFifoPtrs(&fifoObj, &pRead, &pWrite);
        OSSendMessage(&mQueue, pWrite, OS_MESSAGE_BLOCK);
    }
}
