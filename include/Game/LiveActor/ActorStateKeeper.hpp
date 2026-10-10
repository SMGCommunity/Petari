#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/LiveActor/Nerve.hpp"

class ActorStateKeeper {
private:
    struct State {
        /* 0x0 */ ActorStateBaseInterface* mInterface;
        /* 0x4 */ const Nerve* mNerve;
        /* 0x8 */ const char* mName;
    };

public:
    ActorStateKeeper(int capacity);

    void addState(ActorStateBaseInterface*, const Nerve*, const char*);
    bool updateCurrentState();
    void startState(const Nerve*);
    void endState(const Nerve*);
    State* findStateInfo(const Nerve*);

    /* 0x0 */ s32 mStatesCapacity;
    /* 0x4 */ s32 mLength;
    /* 0x8 */ State* mStates;
    /* 0xC */ State* mCurrentState;
};
