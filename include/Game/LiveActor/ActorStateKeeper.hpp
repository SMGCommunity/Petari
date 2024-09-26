#pragma once

#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"

class ActorStateKeeper {
private:
	struct State {
		ActorStateBaseInterface* mInterface;
		const Nerve* mNerve;
		const char* mName;
	};
public:
    ActorStateKeeper(int);

    void addState(ActorStateBaseInterface *, const Nerve *, const char *);
    bool updateCurrentState();
    void startState(const Nerve *);
    void endState(const Nerve *);
    State* findStateInfo(const Nerve *);

    s32 mStatesCapacity;
    s32 mLength;
    State* mStates;           // 0x8
    State* mCurrentState;           // 0xC
};
