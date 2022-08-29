#include "JSystem/JSupport/JSUList.h"

JSUPtrLink::JSUPtrLink(void *pData) {
    mData = pData;
    mPtrList = 0;
    mPrev = 0;
    mNext = 0;
}

JSUPtrLink::~JSUPtrLink() {
    if (mPtrList) {
        mPtrList->remove(this);
    }
}

JSUPtrList::JSUPtrList(bool doInitialize) {
    if (doInitialize) {
        initiate();
    }
}

JSUPtrList::~JSUPtrList() {
    JSUPtrLink* curHead = mHead;

    for (int i = 0; i < mNodeCount; i++) {
        curHead->mPtrList = 0;
        curHead = curHead->mNext;
    }
}

void JSUPtrList::initiate() {
    mHead = 0;
    mTail = 0;
    mNodeCount = 0;
}

void JSUPtrList::setFirst(JSUPtrLink *pLink) {
    pLink->mPtrList = this;
    pLink->mPrev = 0;
    pLink->mNext = 0;
    mTail = pLink;
    mHead = pLink;
    mNodeCount = 1;
}

bool JSUPtrList::append(JSUPtrLink *pLink) {
    bool validity = (pLink->mPtrList == 0);

    if (!validity) {
        validity = pLink->mPtrList->remove(pLink);
    }

    if (validity) {
        if (!mNodeCount) {
            setFirst(pLink);
        }
        else {
            pLink->mPtrList = this;
            pLink->mPrev = mTail;
            pLink->mNext = 0;
            mTail->mNext = pLink;
            mTail = pLink;
            mNodeCount = mNodeCount + 1;
        }
    }

    return validity;
}

bool JSUPtrList::prepend(JSUPtrLink *pLink) {
    bool validity = (pLink->mPtrList == 0);

    if (!validity) {
        validity = pLink->mPtrList->remove(pLink);
    }

    if (validity) {
        if (!mNodeCount) {
            setFirst(pLink);
        }
        else {
            pLink->mPtrList = this;
            pLink->mPrev = 0;
            pLink->mNext = mHead;
            mHead->mPrev = pLink;
            mHead = pLink;
            mNodeCount = mNodeCount + 1;
        }
    }

    return validity;
}

bool JSUPtrList::insert(JSUPtrLink *pLink_1, JSUPtrLink *pLink_2) {
    if (pLink_1 == mHead) {
        return prepend(pLink_2);
    }
    if (!pLink_1) {
        return append(pLink_2);
    }
    if (pLink_1->mPtrList != this) {
        return false;
    }

    JSUPtrList* link2PtrList = pLink_2->mPtrList;

    bool validity = (link2PtrList == 0);

    if (!validity) {
        validity = link2PtrList->remove(pLink_2);
    }

    if (validity) {
        JSUPtrLink* prev = pLink_1->mPrev;
        pLink_2->mPtrList = this;
        pLink_2->mPrev = prev;
        pLink_2->mNext = pLink_1;
        prev->mNext = pLink_2;
        pLink_1->mPrev = pLink_2;
        mNodeCount++;
    }

    return validity;
}

bool JSUPtrList::remove(JSUPtrLink *pLink) {
    bool isSameList = (pLink->mPtrList == this);

    if (isSameList) {
        if (mNodeCount == 1) {
            mHead = 0;
            mTail = 0;
        }
        else if (pLink == mHead) {
            pLink->mNext->mPrev = 0;
            mHead = pLink->mNext;
        }
        else if (pLink == mTail) {
            pLink->mPrev->mNext = 0;
            mTail = pLink->mPrev;
        }
        else {
            pLink->mPrev->mNext = pLink->mNext;
            pLink->mNext->mPrev = pLink->mPrev;
        }

        pLink->mPtrList = 0;
        mNodeCount--;
    }

    return isSameList;
}
