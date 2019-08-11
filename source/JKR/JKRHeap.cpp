#include "JKR/JKRHeap.h"
#include "JUT/JUTException.h"

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