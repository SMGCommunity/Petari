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
        u32 _0;
        u32 _4;
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
    bool dispose(void*, u32);
    void dispose(void*, void*);
    void dispose();
    static void copyMemory(void*, void*, u32);
    static JKRErrorHandler setErrorHandler(JKRErrorHandler);

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

    inline u8* getStart() const { return mStart; }

    inline u8* getEnd() const { return mEnd; }

    static JKRHeap* findFromRoot(void*);

    static void* alloc(u32, int, JKRHeap*);
    static void free(void*, JKRHeap*) NO_INLINE;

    static void destroy(JKRHeap*);

    static JKRHeap* getCurrentHeap() { return sCurrentHeap; }

    static void setAltAramStartAdr(u32);
    static u32 getAltAramStartAdr();

    static bool initArena(char**, u32*, int);

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

    inline void* getStartAddr() const { return (void*)mStart; }

    inline void* getEndAddr() const { return (void*)mEnd; }

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

inline JKRHeap* JKRGetCurrentHeap() {
    return JKRHeap::getCurrentHeap();
}
