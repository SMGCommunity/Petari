#include "JKR/JKRDisposer.h"
#include "JKR/JKRHeap.h"

JKRDisposer::JKRDisposer() : mPointerLinks(this)
{
    JKRHeap* heap = JKRHeap::findFromRoot(this);
    this->mRootHeap = heap;

    if (heap != 0)
    {
        heap->mPtrList.append(&this->mPointerLinks);
    }
}

JKRDisposer::~JKRDisposer()
{
    if (this)
    {
        JKRHeap* heap = this->mRootHeap;

        if (heap != 0)
        {
            heap->mPtrList.remove(&this->mPointerLinks);
        }
    }
}