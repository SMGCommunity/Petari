#pragma once

#include "JSystem/JKernel/JKRHeap.hpp"

class JKRUnitHeap : public JKRHeap {
public:
    static JKRUnitHeap* create(u32, u32, u32, JKRHeap*, bool);

    virtual ~JKRUnitHeap();
    virtual u32 getHeapType();
    virtual bool check();
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
    virtual void state_register(TState*, u32) const;
    virtual bool state_compare(const TState&, const TState&) const;

    void clearBatArea();
    s32 find1FreeBlock(int);
    s32 findFreeBlock(int, u32);
    s32 findFreeBlock_fromHead(u32);
    s32 findFreeBlock_fromTail(u32);
    void* indexToAddress(int);
    s32 addressToIndex(void*);
    bool isUnitUsed(int) const;

    u32 _6C;
    u32 _70;
    u32 _74;
    u8* _78;
    u8* _7C;
    /* 0x80 */ s32 mTotalFreeSize;
    u8 _84;
};
