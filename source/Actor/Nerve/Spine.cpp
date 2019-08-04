#include "Actor/Nerve/Spine.h"

Spine::Spine(void *unk1, const Nerve *nerve)
{
    this->_0 = unk1;
    this->mCurState = nerve;
    this->mNextState = 0;
    this->mNextState = 0;
    this->mStateKeeper = 0;
}

void Spine::update()
{
    this->changeNerve();
    this->mCurState->execute(this);
    this->mNerveStep++;
    this->changeNerve();
}

void Spine::setNerve(const Nerve *nerve)
{
    if (this->mNerveStep > 0)
        this->mCurState->executeOnEnd(this);

    this->mNextState = nerve;
    this->mNerveStep = -1;
}

const Nerve* Spine::getCurrentNerve() const
{
    if (this->mNextState == 0)
        return this->mCurState;

    return this->mNextState;
}

void Spine::changeNerve()
{
    if (this->mNextState == 0)
        return;

    if (this->mStateKeeper != 0)
    {
        this->mStateKeeper->endState(this->mCurState);
        this->mStateKeeper->startState(this->mNextState);
    }

    const Nerve* nextState = this->mNextState;
    this->mNerveStep = 0;
    this->mCurState = nextState;
    this->mNextState = 0;
}

void Spine::initStateKeeper(s32 unk1)
{
    ActorStateKeeper* keeper = new ActorStateKeeper(unk1);
    this->mStateKeeper = keeper;
}