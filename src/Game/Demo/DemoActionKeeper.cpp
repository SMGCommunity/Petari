#include "Game/Demo/DemoActionKeeper.hpp"

void DemoActionInfo::registerCast(LiveActor *pActor) {
    mCastList[mCastCount] = pActor;
    mCastCount++;
}

void DemoActionInfo::registerFunctor(const LiveActor *pActor, const MR::FunctorBase &rFunctor) {
    for (s32 i = 0; i < mCastCount; i++) {
        if (mCastList[i] == pActor) {
            mFunctors[i] = rFunctor.clone(0);
            return;
        }
    }
}

void DemoActionInfo::registerNerve(const LiveActor *pActor, const Nerve *pNerve) {
    for (s32 i = 0; i < mCastCount; i++) {
        if (mCastList[i] == pActor) {
            mNerves[i] = pNerve;
            return;
        }
    }
}