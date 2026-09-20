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

        bool isTempMemBlock() const {
            return mFlags & 0x80;
        }

        int getAlignment() const {
            return mFlags & 0x7f;
        }

        int free(JKRExpHeap* pHeap);

        void* getContent() const {
            return (void*)(this + 1);
        }

        void newGroupId(u8 groupId) {
            mGroupId = groupId;
        }

        /* 0x00 */ u16 mMagic;
        /* 0x02 */ u8 mFlags;
        /* 0x03 */ u8 mGroupId;
        /* 0x04 */ u32 mSize;
        /* 0x08 */ CMemBlock* mPrev;
        /* 0x0C */ CMemBlock* mNext;
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
    void removeUsedBlock(CMemBlock*);
    void recycleFreeBlock(CMemBlock*);
    void joinTwoBlocks(CMemBlock*);
    s32 adjustSize();

    s32 getUsedSize(u8) const;

    static JKRExpHeap* createRoot(int, bool);
    static JKRExpHeap* create(u32, JKRHeap*, bool);
    static JKRExpHeap* create(void*, u32, JKRHeap*, bool);

    static s32 getUsedSize_(JKRExpHeap* pHeap) {
        return pHeap->mSize - pHeap->getTotalFreeSize();
    }
    static void* getState_(TState* pState) {
        return getState_buf_(pState);
    }

    u8 mAllocMode;
    u8 mCurrentGroupId;
    bool mIsExternal;
    void* _70;
    u32 _74;
    /* 0x78 */ CMemBlock* mHeadFreeList;
    /* 0x7C */ CMemBlock* mTailFreeList;
    /* 0x80 */ CMemBlock* mHeadUsedList;
    /* 0x84 */ CMemBlock* mTailUsedList;
};

inline int JKRExpHeap::CMemBlock::free(JKRExpHeap* pHeap) {
    pHeap->removeUsedBlock(this);
    pHeap->recycleFreeBlock(this);
    return 0;
}
