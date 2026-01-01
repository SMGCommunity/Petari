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

    void* mXfbs[3];
    bool mXfbAllocated[3];
    /* 0x10 */ s32 mBufferNum;
    /* 0x14 */ s16 mDrawingXfbIndex;
    /* 0x16 */ s16 mDrawnXfbIndex;
    /* 0x18 */ s16 mDisplayingXfbIndex;
    s32 _1C;

    static JUTXfb* sManager;
};
