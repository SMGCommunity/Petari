#include "JSystem/JSupport/JSUList.h"

JSUPtrLink::JSUPtrLink(void *pPtr)
{
    u32 val = 0;
    mPtr = pPtr;
    mPtrList = 0;
    _8 = 0;
    _C = 0;
}

JSUPtrLink::~JSUPtrLink()
{
    if (!this)
    {
        if (mPtrList != 0)
        {
            mPtrList->remove(this);
        }
    }
}

JSUPtrList::JSUPtrList(bool hasBeenInit)
{
    if (hasBeenInit != 0)
    {
        initiate();
    }
}

JSUPtrList::~JSUPtrList()
{
    if (this != 0)
    {
        JSUPtrLink *pCurLink = mFirst;
        u32 curIdx = 0;

        while(curIdx < mNumPtrs)
        {
            pCurLink->mPtrList = 0;
            curIdx++;
            pCurLink = pCurLink->_C;
        }
    }
}

void JSUPtrList::initiate()
{
    mFirst = 0;
    mNext = 0;
    mNumPtrs = 0;
}

void JSUPtrList::setFirst(JSUPtrLink *pLink)
{
    pLink->mPtrList = this;
    u32 defaultCount = 1;
    pLink->_8 = 0;
    pLink->_C = 0;
    mNext = pLink;
    mFirst = pLink;
    mNumPtrs = defaultCount;
}

template<typename T>
JSULink<T>::~JSULink()
{

}