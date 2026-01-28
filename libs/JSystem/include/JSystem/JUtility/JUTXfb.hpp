#pragma once

#include "JSystem/JUtility/JUTVideo.hpp"

class JKRHeap;

class JUTXfb {
public:
    enum EXfbNumber {
        UNK_0 = 0,
        UNK_1 = 1,
        UNK_2 = 2,
        UNK_3 = 3,
    };

    JUTXfb(void*, void*, void*);
    ~JUTXfb();

    void* getDrawingXfb() const {
        if (mDrawingXfbIndex >= 0) {
            return mXfbs[mDrawingXfbIndex];
        }
        return nullptr;
    }

    void clearIndex();
    void delXfb(int);
    void initiate(void*, void*, void*, JUTXfb::EXfbNumber);

    s16 getDisplayingXfbIndex() const NO_INLINE { return mDisplayingXfbIndex; }

    static JUTXfb* getManager() NO_INLINE { return sManager; }

    static JUTXfb* createManager(void*, void*, void*);
    static void destroyManager();

    s32 getBufferNum() const { return mBufferNum; }
    s16 getDrawnXfbIndex() const { return mDrawnXfbIndex; }
    s32 getSDrawingFlag() const { return mSDrawingFlag; }

    void* getDrawnXfb() const {
        if (mDrawnXfbIndex >= 0)
            return mXfbs[mDrawnXfbIndex];
        return NULL;
    }

    void* getDisplayingXfb() const {
        if (mDisplayingXfbIndex >= 0)
            return mXfbs[mDisplayingXfbIndex];
        return NULL;
    }

    void setDisplayingXfbIndex(s16 index) { mDisplayingXfbIndex = index; }
    void setSDrawingFlag(s32 flag) { mSDrawingFlag = flag; }
    void setDrawnXfbIndex(s16 index) { mDrawnXfbIndex = index; }
    void setDrawingXfbIndex(s16 index) { mDrawingXfbIndex = index; }

    void* mXfbs[3];
    bool mXfbAllocated[3];
    /* 0x10 */ s32 mBufferNum;
    /* 0x14 */ s16 mDrawingXfbIndex;
    /* 0x16 */ s16 mDrawnXfbIndex;
    /* 0x18 */ s16 mDisplayingXfbIndex;
    s32 mSDrawingFlag;

    static JUTXfb* sManager;
};
