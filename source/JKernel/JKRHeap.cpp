#include "JKernel/JKRHeap.h"
#include "JUtility/JUTException.h"
#include "smg.h"

#include<revolution/os.h>

const u8 JKRHeap::_unk_0 = 1;

JKRHeap::JKRHeap(void *ptr, u32 size, JKRHeap *pHeap, bool setErrHandler) : JKRDisposer(), mTree(this), mPtrList()
{
    OSInitMutex(&mMutex);

    // todo -- cmpwi and add are swapped
    _38 = size;
    _30 = ptr;
    _34 = static_cast<u8*>(ptr) + size;

    if (!pHeap)
    {
        JKRHeap::sSystemHeap = this;
        JKRHeap::sCurrentHeap = this;
    }
    else
    {
        JSUTree<JKRHeap>* tree = &mTree;

        if (tree)
        {
            tree = (JSUTree<JKRHeap>*)&tree->mLink;
        }

        mTree.append((JSUPtrLink*)tree);

        if (JKRHeap::sSystemHeap == JKRHeap::sRootHeap)
        {
            JKRHeap::sSystemHeap = this;
        }

        if (JKRHeap::sCurrentHeap == JKRHeap::sRootHeap)
        {
            JKRHeap::sCurrentHeap = this;
        }
    }

    _68 = setErrHandler;

    if ((u8)setErrHandler == 1)
    {
        if (!JKRHeap::mErrorHandler)
        {
            JKRHeap::mErrorHandler = JKRDefaultMemoryErrorRoutine;
        }
    }
    
    _3C = JKRHeap::_unk_0;
    _3D = JKRHeap::_unk_1;
    _69 = 0;
}

bool JKRHeap::initArena(s8 **pRamStart, u32 *pRamSize, s32 maxHeaps)
{
    void* arenaLo = OSGetArenaLo();
    void* arenaHi = OSGetArenaHi();
    OSReport("original arenaLo = %p arenaHi = %p\n", arenaLo, arenaHi);

    if (arenaLo == arenaHi)
    {
        return false;
    }

    void* data = OSInitAlloc(arenaLo, arenaHi, maxHeaps);
    u32 ramEnd = (u32)arenaHi & 0xFFFFFFE0;
    u32 ramStart = ((u32)data + 0x1F) & 0xFFFFFFE0;

    JKRHeap::mCodeStart = (void*)0x80000000;
    JKRHeap::mCodeEnd = (void*)ramStart;
    JKRHeap::mUserRamStart = (void*)ramStart;
    JKRHeap::mUserRamEnd = (void*)ramEnd;
    JKRHeap::mMemorySize = *(u32*)0x80000028;

    OSSetArenaLo((void*)ramEnd);
    OSSetArenaHi((void*)ramEnd);

    *pRamStart = (s8*)ramStart;
    *pRamSize = ramEnd - ramStart;
    return true;
}

JKRHeap* JKRHeap::becomeSystemHeap()
{
    JKRHeap* pInstance = this;
    JKRHeap* pCurSystemHeap = JKRHeap::sSystemHeap;
    JKRHeap::sSystemHeap = pInstance;
    return pCurSystemHeap;
}

JKRHeap* JKRHeap::becomeCurrentHeap()
{
    JKRHeap* pInstance = this;
    JKRHeap* pCurHeap = JKRHeap::sCurrentHeap;
    JKRHeap::sCurrentHeap = pInstance;
    return pCurHeap;
}

void JKRHeap::destroy(JKRHeap *pHeap)
{
    do_destroy();
}

NO_INLINE void* JKRHeap::alloc(u32 a1, s32 a2, JKRHeap *pHeap)
{
    if (pHeap != 0)
    {
        return pHeap->alloc(a1, a2);
    }

    JKRHeap *pCurHeap = JKRHeap::sCurrentHeap;

    if (pCurHeap != 0)
    {
        return pCurHeap->alloc(a1, a2);
    }

    return 0;
}

void* JKRHeap::alloc(u32 a1, s32 a2)
{
    return do_alloc(a1, a2);
}

NO_INLINE void JKRHeap::free(void *pSrc, JKRHeap *pHeap)
{
    if (pHeap == 0)
    {
        pHeap = JKRHeap::findFromRoot(pSrc);

        if (pHeap == 0)
        {
            return;
        }
    }

    pHeap->do_free(pSrc);
}

void JKRHeap::free(void *pSrc)
{
    do_free(pSrc);
}

void JKRHeap::callAllDisposer()
{
    while (true)
    {
        JSUPtrLink* link = mPtrList.mFirst;

        if (!link)
        {
            break;
        }

        // destructor seems to be a virtual function?
        link->~JSUPtrLink();
    }
}

void JKRHeap::freeAll()
{
    do_freeAll();
}

void JKRHeap::freeTail()
{
    do_freeTail();
}

void JKRHeap::resize(void *pSrc, u32 a2)
{
    do_resize(pSrc, a2);
}

/*JKRHeap* JKRHeap::findFromRoot(void *pHeap)
{
    if (!JKRHeap::sRootHeap)
    {
        return 0;
    }

    if (JKRHeap::sRootHeap->_30 > pHeap)
    {
        return JKRHeap::sRootHeap->findAllHeap(pHeap);
    }

    if (pHeap >= JKRHeap::sRootHeap->_34)
    {
        return JKRHeap::sRootHeap->findAllHeap(pHeap);
    }

    return JKRHeap::sRootHeap->findAllHeap(pHeap);
}*/

void JKRDefaultMemoryErrorRoutine(void *mSrc, u32 a2, s32 a3)
{
    JUTException::panic_f("JKRHeap.cpp", 0x355, "%s", "abort\n");
}

void* JKRHeap::setErrorHandler(void (*err)(void *, u32, s32))
{
    if (err == 0)
    {
        err = (void (*)(void *, u32, s32))(*JKRDefaultMemoryErrorRoutine);
    }

    void *pCurErrHandler = JKRHeap::mErrorHandler;
    JKRHeap::mErrorHandler = err;
    return pCurErrHandler;
}

s32 JKRHeap::getMaxAllocatableSize(s32 arg1)
{
    s32 maxFreeBlock = getMaxFreeBlock();
    return ~(arg1 - 1) & (getFreeSize() - ((arg1 - 1) & (arg1 - (maxFreeBlock & 0xF))));
}

void* operator new(size_t size)
{
    return JKRHeap::alloc(size, 4, 0);
}

void* operator new(size_t size, s32 arg2)
{
    return JKRHeap::alloc(size, arg2, 0);
}

void* operator new(size_t size, JKRHeap *pHeap, s32 arg3)
{
    return JKRHeap::alloc(size, arg3, pHeap);
}

void* operator new[](size_t size)
{
    return JKRHeap::alloc(size, 4, 0);
}

void* operator new[](size_t size, s32 arg2)
{
    return JKRHeap::alloc(size, arg2, 0);
}

void* operator new[](size_t size, JKRHeap *pHeap, s32 arg3)
{
    return JKRHeap::alloc(size, arg3, pHeap);
}

void operator delete(void *ptr)
{
    JKRHeap::free(ptr, 0);
}

void operator delete[](void *ptr)
{
    JKRHeap::free(ptr, 0);
}