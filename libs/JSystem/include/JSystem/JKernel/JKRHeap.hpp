#pragma once

#include "Inline.hpp"
#include "JSystem/JKernel/JKRDisposer.hpp"
#include "JSystem/JSupport/JSUList.hpp"
#include <revolution.h>

typedef void (*JKRErrorHandler)(void*, u32, int);
void JKRDefaultMemoryErrorRoutine(void*, u32, int);

class JKRHeap : public JKRDisposer {
public:
    class TState {
    public:
        /* 0x00 */ u32 mUsedSize;
        /* 0x04 */ u32 mCheckCode;
        /* 0x08 */ u32 mBuf;
        /* 0x0C */ u32 field_0xc;
        /* 0x10 */ JKRHeap* mHeap;
        /* 0x14 */ u32 mId;

    public:
        u32 getUsedSize() const {
            return mUsedSize;
        }
        u32 getCheckCode() const {
            return mCheckCode;
        }
        JKRHeap* getHeap() const {
            return mHeap;
        }
        u32 getId() const {
            return mId;
        }
    };

    JKRHeap(void*, u32, JKRHeap*, bool);

    virtual ~JKRHeap();
    virtual void callAllDisposer();
    virtual u32 getHeapType() = 0;
    virtual bool check() = 0;
    virtual bool dump_sort();
    virtual bool dump() = 0;
    virtual void do_destroy() = 0;
    virtual void* do_alloc(u32, int) = 0;
    virtual void do_free(void*) = 0;
    virtual void do_freeAll() = 0;
    virtual void do_freeTail() = 0;
    virtual void do_fillFreeArea() = 0;
    virtual s32 do_resize(void*, u32) = 0;
    virtual s32 do_getSize(void*) = 0;
    virtual s32 do_getFreeSize() = 0;
    virtual void* do_getMaxFreeBlock() = 0;
    virtual s32 do_getTotalFreeSize() = 0;
    virtual s32 do_changeGroupID(u8);
    virtual u8 do_getCurrentGroupId();
    virtual void state_register(TState*, u32) const;
    virtual bool state_compare(const TState&, const TState&) const;
    virtual void state_dump(const TState&) const;

    void* alloc(u32, int);
    JKRHeap* becomeSystemHeap();
    JKRHeap* becomeCurrentHeap();
    void destroy();
    bool dispose(void*, u32);
    void dispose(void*, void*);
    void dispose();

    void freeAll();
    void freeTail();
    s32 resize(void*, u32);
    s32 getFreeSize();
    void* getMaxFreeBlock();

    void free(void*);

    JKRHeap* find(void*) const;
    JKRHeap* findAllHeap(void*) const;
    void dispose_subroutine(u32, u32);
    s32 getTotalFreeSize();

    u32 getMaxAllocatableSize(int a1) NO_INLINE {
        u32 v4 = (u32)getMaxFreeBlock();
        return ~(a1 - 1) & (getFreeSize() - ((a1 - 1) & (a1 - (v4 & 0xF))));
    }

    inline u8* getStart() const {
        return mStart;
    }

    inline u8* getEnd() const {
        return mEnd;
    }

    static void* getState_buf_(TState* state) {
        return &state->mBuf;
    }

    JKRHeap* getParent() {
        return mChildTree.getParent()->getObject();
    }

    void lock() const {
        OSLockMutex(const_cast< OSMutex* >(&mMutex));
    }
    void unlock() const {
        OSUnlockMutex(const_cast< OSMutex* >(&mMutex));
    }

    bool getErrorFlag() const {
        return mErrorFlag;
    }

    void callErrorHandler(void* heap, u32 size, int alignment) {
        if (mErrorHandler) {
            (*mErrorHandler)(heap, size, alignment);
        }
    }

    static void setState_u32ID_(TState* state, u32 id) {
        state->mId = id;
    }

    static void setState_uUsedSize_(TState* state, u32 usedSize) {
        state->mUsedSize = usedSize;
    }

    static void setState_u32CheckCode_(TState* state, u32 checkCode) {
        state->mCheckCode = checkCode;
    }

    static void destroy(JKRHeap*);

    static bool initArena(char** memory, u32* size, int maxHeaps);
    static bool initArena2(char** memory, u32* size, int maxHeaps);
    static void* alloc(u32 size, int alignment, JKRHeap* heap);
    static void free(void* ptr, JKRHeap* heap);
    static s32 resize(void* ptr, u32 size, JKRHeap* heap);
    static s32 getSize(void* ptr, JKRHeap* heap);
    static JKRHeap* findFromRoot(void* ptr);

    static void copyMemory(void* dst, void* src, u32 size);
    static void fillMemory(void* dst, u32 size, u8 value);
    static bool checkMemoryFilled(void* src, u32 size, u8 value);

    static JKRErrorHandler setErrorHandler(JKRErrorHandler errorHandler);
    static void fillMemory(u8*, u32, u8);
    static bool checkMemoryFilled(u8*, u32, u8);

    static void* getCodeStart(void) {
        return mCodeStart;
    }
    static void* getCodeEnd(void) {
        return mCodeEnd;
    }
    static void* getUserRamStart(void) {
        return mUserRamStart;
    }
    static void* getUserRamEnd(void) {
        return mUserRamEnd;
    }
    static u32 getMemorySize(void) {
        return mMemorySize;
    }
    static JKRHeap* getRootHeap() {
        return sRootHeap;
    }

    static JKRHeap* getSystemHeap() {
        return sSystemHeap;
    }
    static JKRHeap* getCurrentHeap() {
        return sCurrentHeap;
    }
    static void setSystemHeap(JKRHeap* heap) {
        sSystemHeap = heap;
    }
    static void setCurrentHeap(JKRHeap* heap) {
        sCurrentHeap = heap;
    }

    static void setAltAramStartAdr(u32);
    static u32 getAltAramStartAdr();

    static JKRHeap* sGameHeap;     // 0x806B70A8
    static JKRHeap* sCurrentHeap;  // 0x806B70AC
    static JKRHeap* sRootHeap;     // 0x806B70B0
    static JKRHeap* sSystemHeap;

    static JKRErrorHandler mErrorHandler;

    static void* mCodeStart;
    static void* mCodeEnd;
    static void* mUserRamStart;
    static void* mUserRamEnd;
    static u32 mMemorySize;

    static u32 ARALT_AramStartAddr;

    inline void* getStartAddr() const {
        return (void*)mStart;
    }

    inline void* getEndAddr() const {
        return (void*)mEnd;
    }

    OSMutex mMutex;  // 0x18
    u8* mStart;      // 0x30
    u8* mEnd;        // 0x34
    u32 mSize;       // 0x38
    u8 _3C;
    u8 _3D;
    u8 _3E;
    u8 _3F;
    JSUTree< JKRHeap > mChildTree;         // 0x40
    JSUList< JKRDisposer > mDisposerList;  // 0x5C
    bool mErrorFlag;                       // 0x68
    u8 _69;
    u8 _6A;
    u8 _6B;
};

#ifdef __MWERKS__
void* operator new(u32, int);
void* operator new(u32, JKRHeap*);
void* operator new(u32, JKRHeap*, int);
void* operator new[](u32, int);

void* operator new[](u32, JKRHeap*, int);
#endif

inline void* JKRAllocFromHeap(JKRHeap* heap, u32 size, int alignment) {
    return JKRHeap::alloc(size, alignment, heap);
}

inline void* JKRAllocFromSysHeap(u32 size, int alignment) {
    return JKRHeap::getSystemHeap()->alloc(size, alignment);
}

inline void JKRFreeToHeap(JKRHeap* heap, void* ptr) {
    JKRHeap::free(ptr, heap);
}

inline void JKRFreeToSysHeap(void* ptr) {
    JKRHeap::getSystemHeap()->free(ptr);
}

inline void JKRFree(void* ptr) {
    JKRHeap::free(ptr, NULL);
}

inline void JKRFillMemory(u8* dst, u32 size, u8 val) {
    JKRHeap::fillMemory(dst, size, val);
}

inline JKRHeap* JKRGetSystemHeap() {
    return JKRHeap::getSystemHeap();
}

inline JKRHeap* JKRGetCurrentHeap() {
    return JKRHeap::getCurrentHeap();
}

inline JKRHeap* JKRSetCurrentHeap(JKRHeap* heap) {
    return heap->becomeCurrentHeap();
}

inline u32 JKRGetMemBlockSize(JKRHeap* heap, void* block) {
    return JKRHeap::getSize(block, heap);
}

inline u32 JKRGetFreeSize(JKRHeap* heap) {
    return heap->getFreeSize();
}

inline void* JKRAlloc(u32 size, int alignment) {
    return JKRHeap::alloc(size, alignment, NULL);
}

inline s32 JKRResizeMemBlock(JKRHeap* heap, void* ptr, u32 size) {
    return JKRHeap::resize(ptr, size, heap);
}

inline JKRHeap* JKRFindHeap(void* ptr) {
    return JKRHeap::findFromRoot(ptr);
}

inline JKRHeap* JKRGetRootHeap() {
    return JKRHeap::getRootHeap();
}

inline JKRErrorHandler JKRSetErrorHandler(JKRErrorHandler errorHandler) {
    return JKRHeap::setErrorHandler(errorHandler);
}
