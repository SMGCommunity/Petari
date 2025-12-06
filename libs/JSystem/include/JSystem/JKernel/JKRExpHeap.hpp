#pragma once

#include "JSystem/JKernel/JKRHeap.hpp"

class JKRExpHeap : public JKRHeap {
public:
    class CMemBlock {
    public:
        void initiate(CMemBlock*, CMemBlock*, u32, u8, u8);
        CMemBlock* allocFore(u32, u8, u8, u8, u8);
        CMemBlock* allocBack(u32, u8, u8, u8, u8);
        static CMemBlock* getHeapBlock(void*);

        void* getContent() const { return (void*)(this + 1); }

        u16 mMagic;        // 0x0
        u8 mFlags;         // 0x2
        u8 mGroupId;       // 0x3
        u32 mSize;         // 0x4
        CMemBlock* mPrev;  // 0x8
        CMemBlock* mNext;  // 0xC
    };

    JKRExpHeap(void*, u32, JKRHeap*, bool);

    virtual ~JKRExpHeap();
    virtual u32 getHeapType();
    virtual bool check();
    virtual bool dump_sort();
    virtual bool dump();
    virtual void do_destroy();
    virtual void* do_alloc(u32, int);
    virtual void do_free(void*);
    virtual void do_freeAll();
    virtual void do_freeTail();
    virtual void do_fillFreeArea();
    virtual s32 do_resize(void*, u32);
    virtual s32 do_getSize(void*);
    virtual s32 do_getFreeSize();
    virtual void* do_getMaxFreeBlock();
    virtual s32 do_getTotalFreeSize();
    virtual s32 do_changeGroupID(u8);
    virtual u8 do_getCurrentGroupId();
    virtual void state_register(TState*, u32) const;
    virtual bool state_compare(const TState&, const TState&) const;

    void* allocFromHead(u32);
    void* allocFromHead(u32, int);
    void* allocFromTail(u32);
    void* allocFromTail(u32, int);
    bool isEmpty();

    void appendUsedList(JKRExpHeap::CMemBlock*);
    void setFreeBlock(CMemBlock*, CMemBlock*, CMemBlock*);
    void removeFreeBlock(CMemBlock*);

    static JKRExpHeap* create(void*, u32, JKRHeap*, bool);
    static JKRExpHeap* create(u32, JKRHeap*, bool);

    static JKRExpHeap* createRoot(int, bool);

    u8 mAllocMode;       // 0x6C
    u8 mCurrentGroupId;  // 0x6D
    u8 _6E;
    void* _70;
    u32 _74;
    CMemBlock* mHeadFreeList;  // 0x78
    CMemBlock* mTailFreeList;  // 0x7C
    CMemBlock* mHeadUsedList;  // 0x80
    CMemBlock* mTailUsedList;  // 0x84
};