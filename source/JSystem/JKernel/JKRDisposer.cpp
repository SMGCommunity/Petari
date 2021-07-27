#include "JSystem/JKernel/JKRDisposer.h"
#include "JSystem/JKernel/JKRHeap.h"

JKRDisposer::JKRDisposer() : mPointerLinks(this)
{
    JKRHeap* pHeap = JKRHeap::findFromRoot(this);
    mRootHeap = pHeap;

    if (pHeap != 0)
    {
        pHeap->mPtrList.append(&mPointerLinks);
    }
}

JKRDisposer::~JKRDisposer()
{
    JKRHeap* pHeap = mRootHeap;

    if (pHeap != 0)
    {
        pHeap->mPtrList.remove(&mPointerLinks);
    }
}