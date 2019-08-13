#include "JKR/JKRHeap.h"
#include "JUT/JUTException.h"

JKRHeap* JKRHeap::becomeSystemHeap()
{
    JKRHeap* instance = this;
    JKRHeap* curSystemHeap = JKRHeap::sSystemHeap;
    JKRHeap::sSystemHeap = instance;
    return curSystemHeap;
}

JKRHeap* JKRHeap::becomeCurrentHeap()
{
    JKRHeap* instance = this;
    JKRHeap* curHeap = JKRHeap::sCurrentHeap;
    JKRHeap::sCurrentHeap = instance;
    return curHeap;
}

void JKRHeap::destroy(JKRHeap* heap)
{
    this->do_destroy();
}

JKRHeap* JKRHeap::alloc(u32 a1, s32 a2, JKRHeap *heap)
{
    if (heap != 0)
    {
        return heap->alloc(a1, a2);
    }

    JKRHeap* curHeap = JKRHeap::sCurrentHeap;

    if (curHeap != 0)
    {
        return curHeap->alloc(a1, a2);
    }

    return 0;
}

JKRHeap* JKRHeap::alloc(u32 a1, s32 a2)
{
    return this->do_alloc(a1, a2);
}

void JKRHeap::free(void *src, JKRHeap *heap)
{
    if (heap == 0)
    {
        heap = JKRHeap::findFromRoot(src);

        if (heap == 0)
        {
            return;
        }
    }

    heap->do_free(src);
}

void JKRHeap::free(void *src)
{
    this->do_free(src);
}

void JKRHeap::freeAll()
{
    this->do_freeAll();
}

void JKRHeap::freeTail()
{
    this->do_freeTail();
}

void JKRHeap::resize(void *src, u32 a2)
{
    this->do_resize(src, a2);
}

void JKRHeap::copyMemory(void* dest, void* src, u32 len)
{
    __asm
    {
        addi r0, r5, 3
        srwi. r0, r0, 2
        mtctr r0
        beqlr

    loop:
        lwz r0, 0(r4)
        addi r4, r4, 4
        stw r0, 0(r3)
        addi r3, r3, 4
        bdnz loop
        blr
    }
}

void JKRDefaultMemoryErrorRoutine(void *src, u32 a2, s32 a3)
{
    JUTException::panic_f("JKRHeap.cpp", 0x355, "%s", "abort\n");
}

void* JKRHeap::setErrorHandler(void (*err)(void *, u32, s32))
{
    if (err == 0)
    {
        err = (void (*)(void *, u32, s32))(*JKRDefaultMemoryErrorRoutine);
    }

    void* curErrHandler = JKRHeap::sErrorHandler;
    JKRHeap::sErrorHandler = err;
    return curErrHandler;
}