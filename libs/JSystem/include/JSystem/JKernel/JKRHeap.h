#pragma once

#include<revolution.h>
#include "JSystem/JSupport/JSUList.h"
#include "JSystem/JKernel/JKRDisposer.h"
#include "Inline.h"

// this inherits JKRDisposer
class JKRHeap : public JKRDisposer {
public:

    class TState {
    public:
        u32 _0;
        u32 _4;
    };

    JKRHeap(void *, u32, JKRHeap *, bool);

    virtual ~JKRHeap();
    virtual void callAllDisposer();
    virtual u32 getHeapType() = 0;
    virtual bool check() = 0;
    virtual bool dump_sort();
    virtual bool dump() = 0;
    virtual void do_destroy() = 0;
    virtual void* do_alloc(u32, int) = 0;
    virtual void do_free(void *) = 0;
    virtual void do_freeAll() = 0;
    virtual void do_freeTail() = 0;
    virtual void do_fillFreeArea() = 0;
    virtual s32 do_resize(void *, u32) = 0;
    virtual s32 do_getSize(void *) = 0;
    virtual s32 do_getFreeSize() = 0;
    virtual void* do_getMaxFreeBlock() = 0;
    virtual s32 do_getTotalFreeSize() = 0;
    virtual s32 do_changeGroupID(u8);
    virtual u8 do_getCurrentGroupId();
    virtual void state_register(TState *, u32) const;
    virtual bool state_compare(const TState &, const TState &) const;
    virtual void state_dump(const TState &) const;

    void* alloc(u32, int);
    JKRHeap* becomeSystemHeap();
    JKRHeap* becomeCurrentHeap();

    void freeAll();
    void freeTail();
    s32 resize(void *, u32);
    s32 getFreeSize();
    void* getMaxFreeBlock();

    void free(void *);

    JKRHeap* find(void *) const;
    JKRHeap* findAllHeap(void *) const;
    void dispose_subroutine(u32, u32);

    static JKRHeap* findFromRoot(void *);


    static void* alloc(u32, int, JKRHeap *);
    static void free(void *, JKRHeap *) NO_INLINE;

    static void destroy(JKRHeap *);

    static void setAltAramStartAdr(u32);

    static void setErrorHandler(void (*)(void *, u32, int));

    static void initArena(char **, u32 *, int);

    static JKRHeap *sGameHeap; // _806B70A8
    static JKRHeap *sCurrentHeap; // _806B70AC
    static JKRHeap *sRootHeap; // _806B70B0
    static JKRHeap *sSystemHeap;

    static u32 ARALT_AramStartAddr;

    inline void* getStartAddr() const {
        return (void*)mStart;
    }

    inline void* getEndAddr() const {
        return (void*)mEnd;
    }

    OSMutex mMutex;                     // _18
    u8* mStart;                         // _30
    u8* mEnd;                           // _34
    u32 mSize;                          // _38
    u8 _3C;
    u8 _3D;
    u8 _3E;
    u8 _3F;
    JSUTree<JKRHeap> mChildTree;        // _40
    JSUList<JKRDisposer> mDisposerList; // _5C
    u8 _68;
    u8 _69;
    u8 _6A;
    u8 _6B;
};

void* operator new(u32, JKRHeap *, int);
void* operator new[](u32, int);

void* operator new[](u32, JKRHeap *, int);
