#pragma once

#include "Game/System/NerveExecutor.hpp"

class ActorStateBaseInterface : public NerveExecutor {
public:
    ActorStateBaseInterface(const char* pName) : NerveExecutor(pName) {}

    virtual ~ActorStateBaseInterface() {}

    virtual void init() {}

    virtual void appear() { mIsDead = false; }

    virtual void kill() { mIsDead = true; }

    virtual bool update();

    virtual void control() {}

    /* 0x8 */ bool mIsDead;
};

template < typename T >
class ActorStateBase : public ActorStateBaseInterface {
public:
    ActorStateBase(const char* pName) : ActorStateBaseInterface(pName) {}

    virtual ~ActorStateBase() NO_INLINE {}
};
