#include "Game/LiveActor/ActorStateKeeper.hpp"
#include "Game/LiveActor/Nerve.hpp"

ActorStateKeeper::ActorStateKeeper(int capacity)
	: mStatesCapacity(capacity), mLength(0), mStates(NULL), mCurrentState(NULL) {
	mStates = new State [capacity];
	for(s32 i = 0; i < mStatesCapacity; i++) {
		State& e = mStates[i];
		e.mInterface = NULL;
		e.mNerve = NULL;
		e.mName = NULL;
	}
}

bool ActorStateKeeper::updateCurrentState() {
    return (!mCurrentState) ? false : (mCurrentState->mInterface)->update();
}

void ActorStateKeeper::startState(const Nerve *pNerve) {
    mCurrentState = findStateInfo(pNerve);

    if (mCurrentState) {
        ActorStateBaseInterface* interface = mCurrentState->mInterface;
        interface->appear();
    }
}

void ActorStateKeeper::endState(const Nerve *pNerve) {
    mCurrentState = findStateInfo(pNerve);

    if (mCurrentState) {
        ActorStateBaseInterface* interface = mCurrentState->mInterface;
        if (!interface->mIsDead) {
            interface->kill();
        }
    }
}

void ActorStateKeeper::addState(ActorStateBaseInterface* pInterface, const Nerve* pNerve, const char* pName) {
	State& e = mStates[mLength];
	e.mInterface = pInterface;
	e.mNerve = pNerve;
	e.mName = pName;
	mLength += 1;
}

ActorStateKeeper::State* ActorStateKeeper::findStateInfo(const Nerve *pNerve) {
	for(int i = 0; i < mLength; i++) {
		if(mStates[i].mNerve == pNerve) return &mStates[i];
	}
	return NULL;
}
