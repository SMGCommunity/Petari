#include "Game/LiveActor/ActiveActorList.h"
#include "Game/LiveActor/LiveActor.h"

ActiveActorList::ActiveActorList(int max) {
    mCurCount = max;
    mActorList = 0;
    mMaxCount = 0;
    mActorList = new LiveActor*[max];
    clear();
}

bool ActiveActorList::isFull() const {
    return mCurCount >= mMaxCount;
}

#ifdef NON_MATCHING
void ActiveActorList::addActor(LiveActor *pActor) {
    s32 count = mCurCount;
    if (count < mMaxCount) {
        mActorList[mCurCount] = pActor;
        mCurCount++;
    }
}
#endif

void ActiveActorList::removeDeadActor() {
    s32 cur = 0;
    s32 idx = 0;

    while (cur < mCurCount) {
        if (MR::isDead(mActorList[idx])) {
            mActorList[idx] = mActorList[mCurCount - 1];
            s32 newCount = mCurCount - 1;
            mActorList[newCount] = 0;
            mCurCount--;
        }
        else {
            ++cur;
            ++idx;
        }
    }
}

void ActiveActorList::clear() {
    for (s32 i = 0; i < mMaxCount; i++) {
        mActorList[i] = 0;
    }

    mCurCount = 0;
}

void ActiveActorList::killAll() {
    for (s32 i = 0; i < mCurCount; i++) {
        if (!MR::isDead(mActorList[i])) {
            mActorList[i]->kill();
        }

        mActorList[i] = 0;
    }

    mCurCount = 0;
}