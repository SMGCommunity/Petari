#pragma once

#include "JSystem/JKernel/JKRHeap.h"

class JKRExpHeap : public JKRHeap {
public:

    class CMemBlock {
    public:

        void initiate(CMemBlock *, CMemBlock *, u32, u8, u8);
        CMemBlock* allocFore(u32, u8, u8, u8, u8);
        CMemBlock* allocBack(u32, u8, u8, u8, u8);
        static CMemBlock* getHeapBlock(void *);

        u16 mMagic;             // _0
        u8 mFlags;              // _2
        u8 mGroupId;            // _3
        u32 mSize;              // _4
        CMemBlock* mPrev;       // _8
        CMemBlock* mNext;       // _C
    };

    JKRExpHeap(void *, u32, JKRHeap *, bool);

    virtual ~JKRExpHeap();
    virtual u32 getHeapType();
    virtual bool check();
    virtual bool dump_sort();
    virtual bool dump();
    virtual void do_destroy();
    virtual void* do_alloc(u32, int);
    virtual void do_free(void *);
    virtual void do_freeAll();
    virtual void do_freeTail();
    virtual void do_fillFreeArea();
    virtual s32 do_resize(void *, u32);
    virtual s32 do_getSize(void *);
    virtual s32 do_getFreeSize();
    virtual void* do_getMaxFreeBlock();
    virtual s32 do_getTotalFreeSize();
    virtual s32 do_changeGroupID(u8);
    virtual u8 do_getCurrentGroupId();
    virtual void state_register(TState *, u32) const;
    virtual bool state_compare(const TState &, const TState &) const;

    void* allocFromHead(u32);
    void* allocFromHead(u32, int);
    void* allocFromTail(u32);
    void* allocFromTail(u32, int);

    static JKRExpHeap* create(void *, u32, JKRHeap *, bool);
    static JKRExpHeap* create(u32, JKRHeap *, bool);

    static JKRExpHeap* createRoot(int, bool);

    u8 mAllocMode;          // _6C
    u8 mCurrentGroupId;     // _6D
    u8 _6E;
    void* _70;
    u32 _74;
    CMemBlock* mHeadFreeList;   // _78
    CMemBlock* mTailFreeList;   // _7C
    CMemBlock* mHeadUsedList;   // _80
    CMemBlock* mTailUsedList;   // _84
};