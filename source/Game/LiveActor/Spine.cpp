#include "Game/LiveActor/Spine.h"
#include "Game/LiveActor/ActorStateKeeper.h"

Spine::Spine(void *pExecutor, const Nerve *pNerve) {
    mExecutor = pExecutor;
    mCurNerve = pNerve;
    mNextNerve = nullptr;
    mStep = 0;
    mStateKeeper = 0;
}

void Spine::update() {
    changeNerve();
    mCurNerve->execute(this);
    mStep++;
    changeNerve();
}

void Spine::setNerve(const Nerve *pNerve) {
    if (mStep > 0) {
        mCurNerve->executeOnEnd(this);
    }

    mNextNerve = pNerve;
    mStep = -1;
}

const Nerve* Spine::getCurrentNerve() const{
    if (mNextNerve) {
        return mNextNerve;
    }
    return mCurNerve;
}

void Spine::changeNerve() {
    if (mNextNerve == nullptr) {
        return;
    }

    if (mStateKeeper != 0) {
        mStateKeeper->endState(mCurNerve);
        mStateKeeper->startState(mNextNerve);
    }

    const Nerve* pNextState = mNextNerve;
    mStep = 0;
    mCurNerve = pNextState;
    mNextNerve = nullptr;
}

void Spine::initStateKeeper(int a2) {
    mStateKeeper = new ActorStateKeeper(a2);
}