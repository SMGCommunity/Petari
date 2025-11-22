#pragma once

class JUTXfb {
public:
    void* getDrawingXfb() const {
        if (mDrawingXfbIndex >= 0) {
            return mXfbs[mDrawingXfbIndex];
        }
        return nullptr;
    }

    s16 getDisplayingXfbIndex() const NO_INLINE {
        return mDisplayingXfbIndex;
    }

    static JUTXfb* getManager() NO_INLINE {
        return sManager;
    }

    static JUTXfb* createManager(void*, void*, void*);
    static void    destroyManager();

    void*          mXfbs[3];
    u8             _C;
    u8             _D;
    u8             _E;
    /* 0x10 */ s32 mBufferMode;
    /* 0x14 */ s16 mDrawingXfbIndex;
    /* 0x16 */ s16 mLastDrawnXfbIndex;
    /* 0x18 */ s16 mDisplayingXfbIndex;
    s32            _1C;

    static JUTXfb* sManager;
};
