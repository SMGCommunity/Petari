#include "JKernel/JKRDisposer.h"
#include "JKernel/JKRHeap.h"

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
    if (this)
    {
        JKRHeap* pHeap = mRootHeap;

        if (pHeap != 0)
        {
            pHeap->mPtrList.remove(&mPointerLinks);
        }
    }
}