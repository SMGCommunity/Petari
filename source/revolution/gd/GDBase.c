#include "revolution/GD/GDBase.h"
#include "revolution/os/OSCache.h"

void GDInitGDLObj(GDLObj *pObj, void *pStart, u32 len)
{
    pObj->mStart = pStart;
    pObj->mPtr = pStart;
    pObj->mTop = (u8*)pStart + len;
    pObj->mLength = len;
}

void GDFlushCurrToMem()
{
    DCFlushRange(__GDCurrentDL->mStart, __GDCurrentDL->mLength);
}

void GDOverflowCheck(u32 numBytes)
{
    // checks to see if inserting numBytes will cause an overflow in the display list
    if (__GDCurrentDL->mPtr + numBytes > __GDCurrentDL->mTop)
    {
        GDOverflowed();
    }
}