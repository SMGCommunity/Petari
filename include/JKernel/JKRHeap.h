#pragma once

#include "JKernel/JKRDisposer.h"
#include "JSupport/JSUList.h"
#include "JSupport/JSUTree.h"

#include <stddef.h>
#include <revolution/os.h>

class JKRHeap : public JKRDisposer
{
public:
    JKRHeap(void *, u32, JKRHeap *, bool);

    virtual ~JKRHeap();
    virtual void callAllDisposer();
    virtual s32 getHeapType() = 0;
    virtual void check() = 0;
    virtual s32 dump_sort();
    virtual JKRHeap* dump() = 0;
    virtual void do_destroy() = 0;
    virtual JKRHeap* do_alloc(u32, s32) = 0;
    virtual void do_free(void *) = 0;
    virtual void do_freeAll() = 0;
    virtual void do_freeTail() = 0;
    virtual void do_fillFreeArea() = 0;
    virtual s32 do_resize(void *, u32) = 0;
    virtual s32 do_getSize(void *) = 0;
    virtual s32 do_getFreeSize() = 0;
    virtual s32 do_getMaxFreeBlock() = 0;
    virtual s32 do_getTotalFreeSize() = 0;

    static bool initArena(s8 **, u32 *, s32);

    JKRHeap* becomeSystemHeap();
    JKRHeap* becomeCurrentHeap();
    void destroy(JKRHeap *pHeap = NULL);
    static void* alloc(u32, s32, JKRHeap *);
    void* alloc(u32, s32);
    static void free(void *, JKRHeap *);
    void free(void *);
    void freeAll();
    void freeTail();
    void resize(void *, u32);
    static JKRHeap* findFromRoot(void *);
    static void copyMemory(void *dest, void *src, u32 len);
    static void* setErrorHandler(void (*)(void *, u32, s32));
    JKRHeap* findAllHeap(void *) const;
    JKRHeap* find(void *) const;
    s32 getMaxAllocatableSize(s32);
    s32 getMaxFreeBlock();
    s32 getFreeSize();

    static void setAltAramStartAdr(u32);
    static u32 getAltAramStartAdr();
    
    OSMutex mMutex; // _18
    void* _30;
    void* _34;
    u32 _38;
    u8 _3C;
    u8 _3D;
    u8 _3E;
    u8 _3F;
    JSUTree<JKRHeap> mTree; // _40
    JSUPtrList mPtrList; // _5C
    bool _68;
    bool _69;

    static void* mCodeStart;
    static void* mCodeEnd;
    static void* mUserRamStart;
    static void* mUserRamEnd;
    static s32 mMemorySize;

    static JKRHeap* sSystemHeap;
    static JKRHeap* sCurrentHeap;
    static JKRHeap* sRootHeap;
    static void* mErrorHandler;

    const static u8 _unk_0 = 1;
    const static u8 _unk_1;
};

void JKRDefaultMemoryErrorRoutine(void *, u32, s32);

void* operator new(size_t size);
void* operator new(size_t, int);
void* operator new(size_t, JKRHeap *, int);

void* operator new[](size_t);
void* operator new[](size_t, int);
void* operator new[](size_t, JKRHeap *, int);

void operator delete(void *);
void operator delete[](void *);

// for some reason these are illegal?
/*
void* operator new[](s32 size);
void* operator new[](s32 size, s32 align);
void* operator new[](s32 size, JKRHeap* heap, s32 align);
*/