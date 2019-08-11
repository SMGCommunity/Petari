#include "JSU/JSUList.h"

JSUPtrLink::JSUPtrLink(void* ptr)
{
    u32 val = 0;
    this->mPtr = ptr;
    this->mPtrList = 0;
    this->_8 = 0;
    this->_C = 0;
}

JSUPtrLink::~JSUPtrLink()
{
    if (!this)
    {
        if (this->mPtrList != 0)
        {
            this->mPtrList->remove(this);
        }
    }
}

JSUPtrList::JSUPtrList(bool hasBeenInit)
{
    if (hasBeenInit != 0)
    {
        this->initiate();
    }
}

JSUPtrList::~JSUPtrList()
{
    if (this != 0)
    {
        JSUPtrLink* curLink = this->mFirst;
        u32 curIdx = 0;

        while(curIdx < this->mNumPtrs)
        {
            curLink->mPtrList = 0;
            curIdx++;
            curLink = curLink->_C;
        }
    }
}

void JSUPtrList::initiate()
{
    this->mFirst = 0;
    this->mNext = 0;
    this->mNumPtrs = 0;
}

void JSUPtrList::setFirst(JSUPtrLink *link)
{
    link->mPtrList = this;
    u32 defaultCount = 1;
    link->_8 = 0;
    link->_C = 0;
    this->mNext = link;
    this->mFirst = link;
    this->mNumPtrs = defaultCount;
}