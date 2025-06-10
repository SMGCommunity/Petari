#include "Game/Util.hpp"

namespace MR {
    BothDirPtrLink::BothDirPtrLink(void *pLink) :
        mValue(pLink),
        mList(nullptr),
        mPrevLink(nullptr),
        mNextLink(nullptr)
    {

    }

    BothDirPtrLink::~BothDirPtrLink() {
        if (mList) {
            mList->remove(this);
        }
    }

    BothDirPtrList::BothDirPtrList(bool doInit) {
        if (doInit) {
            initiate();
        }
    }

    BothDirPtrList::~BothDirPtrList() {
        BothDirPtrLink* pLink = mHead;
        u32 i = 0;
        while (i < mCount) {
            pLink->mList = nullptr;
            i++;
            pLink = pLink->mNextLink;
        }
    }

    void BothDirPtrList::initiate() {
        mHead = nullptr;
        mTail = nullptr;
        mCount = 0;
    }

    void BothDirPtrList::setFirst(BothDirPtrLink *pFirst) {
        pFirst->mList = this;
        pFirst->mPrevLink = nullptr;
        pFirst->mNextLink = nullptr;
        mTail = pFirst;
        mHead = pFirst;
        mCount = 1;
    }

    bool BothDirPtrList::append(BothDirPtrLink* pLink) {
        bool success = !pLink->mList;
        if (!success) {
            success = pLink->mList->remove(pLink);
        }
        if (success) {
            if (mCount == 0) {
                setFirst(pLink);
            }
            else {
                pLink->mList = this;
                pLink->mPrevLink = mTail;
                pLink->mNextLink = nullptr;

                mTail->mNextLink = pLink;
                mTail = pLink;
                mCount++;
            }
        }
        return success;
    }

    bool BothDirPtrList::remove(BothDirPtrLink* pLink) {
        bool success = pLink->mList == this;
        if (success) {
            if (mCount == 1) {
                mHead = nullptr;
                mTail = nullptr;
            }
            else if (pLink == mHead) {
                pLink->mNextLink->mPrevLink = nullptr;
                mHead = pLink->mNextLink;
            }
            else if (pLink == mTail) {
                pLink->mPrevLink->mNextLink = nullptr;
                mTail = pLink->mPrevLink;
            }
            else {
                pLink->mPrevLink->mNextLink = pLink->mNextLink;
                pLink->mNextLink->mPrevLink = pLink->mPrevLink;
            }
            pLink->mList = nullptr;
            mCount--;
        }
        return success;
    }
};
