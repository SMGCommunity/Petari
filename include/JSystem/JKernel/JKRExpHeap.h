#pragma once

#include "JSystem/JKernel/JKRHeap.h"

class JKRExpHeap : public JKRHeap {
public:

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

    static JKRExpHeap* create(void *, u32, JKRHeap *, bool);
    static JKRExpHeap* create(u32, JKRHeap *, bool);

    static JKRExpHeap* createRoot(int, bool);

    u8 _6C;
    u32 _70;
    u32 _74;
    u32 _78;
    u32 _7C;
    u32 _80;
};