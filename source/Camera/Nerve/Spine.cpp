#include "LiveActor/Nerve/Spine.h"
#include "LiveActor/State/ActorStateKeeper.h"

Spine::Spine(void *pExecutor, const Nerve *pNerve)
{
    mExecutor = pExecutor;
    mCurState = pNerve;
    mNextState = 0;
    mNextState = 0;
    mStateKeeper = 0;
}

void Spine::update()
{
    changeNerve();
    mCurState->execute(this);
    mNerveStep++;
    changeNerve();
}

void Spine::setNerve(const Nerve *pNerve)
{
    if (mNerveStep > 0)
    {
        mCurState->executeOnEnd(this);
    }

    mNextState = pNerve;
    mNerveStep = -1;
}

const Nerve* Spine::getCurrentNerve() const
{
    if (mNextState == 0)
        return mCurState;

    return mNextState;
}

void Spine::changeNerve()
{
    if (mNextState == 0)
    {
        return;
    }

    if (mStateKeeper != 0)
    {
        mStateKeeper->endState(mCurState);
        mStateKeeper->startState(mNextState);
    }

    const Nerve* pNextState = mNextState;
    mNerveStep = 0;
    mCurState = pNextState;
    mNextState = 0;
}

void Spine::initStateKeeper(s32 unk1)
{
    ActorStateKeeper* pKeeper = new ActorStateKeeper(unk1);
    mStateKeeper = pKeeper;
}