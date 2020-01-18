#include "OS/GD/GDBase.h"
#include "OS/OSCache.h"

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