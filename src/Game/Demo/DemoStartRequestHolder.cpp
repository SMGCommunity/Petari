#include "Game/Demo/DemoStartRequestHolder.hpp"
#include "Game/Util/StringUtil.hpp"

DemoStartInfo::DemoStartInfo() {
    _0 = 0;
    _4 = 0;
    _8 = 0;
    _C = 0;
    _10 = 0;
    _14 = 0;
    mDemoName = 0;
    _1C = 0;
    _20 = 0;
    _24 = 0;
    _28 = 0;
    _2C = 0;
    _30 = 0;
    _34 = 0;
}

DemoStartInfo& DemoStartInfo::operator=(const DemoStartInfo& rOther) {
    _0 = rOther._0;
    _4 = rOther._4;
    _8 = rOther._8;
    _C = rOther._C;
    _10 = rOther._10;
    _14 = rOther._14;
    mDemoName = rOther.mDemoName;
    _1C = rOther._1C;
    _20 = rOther._20;
    _24 = rOther._24;
    _28 = rOther._28;
    _2C = rOther._2C;
    _30 = rOther._30;
    _34 = rOther._34;
    return *this;
}

void DemoStartRequestHolder::pushRequest(LiveActor* pActor, const char* pName) {
    const DemoStartInfo* pInfo = find(pActor, pName);
    if (!pInfo) {
        pInfo = findEmpty();
    }
    mRequestBuffer.push_back(pInfo);
}

void DemoStartRequestHolder::pushRequest(LayoutActor* pActor, const char* pName) {
    const DemoStartInfo* pInfo = find(pActor, pName);
    if (!pInfo) {
        pInfo = findEmpty();
    }
    mRequestBuffer.push_back(pInfo);
}

void DemoStartRequestHolder::pushRequest(NerveExecutor* pExecutor, const char* pName) {
    const DemoStartInfo* pInfo = find(pExecutor, pName);
    if (!pInfo) {
        pInfo = findEmpty();
    }
    mRequestBuffer.push_back(pInfo);
}

void DemoStartRequestHolder::pushRequest(NameObj* pObj, const char* pName) {
    const DemoStartInfo* pInfo = find(pObj, pName);
    if (!pInfo) {
        pInfo = findEmpty();
    }
    mRequestBuffer.push_back(pInfo);
}

void DemoStartRequestHolder::popRequest() {
    DemoStartInfo** pIter = mStartInfos;
    const DemoStartInfo** pCurReqPtr = mRequestBuffer.mHead.mHead;
    DemoStartInfo** pEnd = &mStartInfos[mNumInfos];

    goto check;
    do {
        pIter++;
check:
        if (pIter == pEnd) {
            break;
        }
    } while (*pIter != *pCurReqPtr);

    if (pIter != pEnd) {
        DemoStartInfo* pFound = *pIter;
        DemoStartInfo emptyInfo;
        *pFound = emptyInfo;
    }

    if (mRequestBuffer.mCount != 0) {
        ++mRequestBuffer.mHead;
        mRequestBuffer.mCount--;
    }
}

bool DemoStartRequestHolder::isExistRequest() const {
    return mRequestBuffer.mCount != 0;
}

const DemoStartInfo* DemoStartRequestHolder::getCurrentInfo() const {
    if (mRequestBuffer.mCount == 0) {
        return 0;
    }

    return *mRequestBuffer.mHead.mHead;
}

void DemoStartRequestHolder::registerStartDemoInfo(const DemoStartInfo& rInfo) {
    DemoStartInfo* pEmpty = findEmpty();
    *pEmpty = rInfo;
}

DemoStartInfo* DemoStartRequestHolder::find(const LiveActor* pActor, const char* pName) const {
    for (DemoStartInfo* const* pIter = mStartInfos; pIter != &mStartInfos[mNumInfos]; pIter++) {
        if ((*pIter)->_0 == (u32)pActor && MR::isEqualString((*pIter)->mDemoName, pName)) {
            return *pIter;
        }
    }
    return 0;
}

DemoStartInfo* DemoStartRequestHolder::find(const LayoutActor* pActor, const char* pName) const {
    for (DemoStartInfo* const* pIter = mStartInfos; pIter != &mStartInfos[mNumInfos]; pIter++) {
        if ((*pIter)->_4 == (u32)pActor && MR::isEqualString((*pIter)->mDemoName, pName)) {
            return *pIter;
        }
    }
    return 0;
}

DemoStartInfo* DemoStartRequestHolder::find(const NerveExecutor* pExecutor, const char* pName) const {
    for (DemoStartInfo* const* pIter = mStartInfos; pIter != &mStartInfos[mNumInfos]; pIter++) {
        if ((*pIter)->_8 == (u32)pExecutor && MR::isEqualString((*pIter)->mDemoName, pName)) {
            return *pIter;
        }
    }
    return 0;
}

DemoStartInfo* DemoStartRequestHolder::find(const NameObj* pObj, const char* pName) const {
    for (DemoStartInfo* const* pIter = mStartInfos; pIter != &mStartInfos[mNumInfos]; pIter++) {
        if ((*pIter)->_C == (u32)pObj && MR::isEqualString((*pIter)->mDemoName, pName)) {
            return *pIter;
        }
    }
    return 0;
}

DemoStartInfo* DemoStartRequestHolder::findEmpty() const {
    for (DemoStartInfo* const* pIter = mStartInfos; pIter != &mStartInfos[mNumInfos]; pIter++) {
        if (DemoStartRequestUtil::isEmpty(*pIter)) {
            DemoStartInfo* pFound = *pIter;
            DemoStartInfo emptyInfo;
            *pFound = emptyInfo;
            return *pIter;
        }
    }
    return 0;
}

DemoStartRequestHolder::DemoStartRequestHolder() : mNumInfos(0), mRequestBuffer(mRequestBuffer.mBuffer, mRequestBuffer.mBuffer) {
    mProxyObj = new NameObj("\xE4\xBB\xA3\xE7\x90\x86\xE4\xBA\xBA");

    for (u32 i = 0; i < 16; i++) {
        DemoStartInfo* pInfo = new DemoStartInfo();
        s32 idx = mNumInfos;
        mNumInfos = idx + 1;
        mStartInfos[idx] = pInfo;
    }
}

template<>
MR::FixedRingBuffer<const DemoStartInfo*, 16>::iterator::iterator(const DemoStartInfo** head, const DemoStartInfo** tail) {
    mHead = head;
    mTail = tail;
    mEnd = tail + 16;
}

template<>
void MR::FixedRingBuffer<const DemoStartInfo*, 16>::push_back(const DemoStartInfo* const& val) {
    if ((u32)mCount >= 16) {
        return;
    }

    *mEnd.mHead = val;
    ++mEnd;
    mCount++;
}

template<>
void MR::FixedRingBuffer<const DemoStartInfo*, 16>::iterator::operator++() {
    const DemoStartInfo** pEnd = mEnd;
    mHead++;

    if (pEnd <= mHead) {
        mHead = mTail;
    }
}