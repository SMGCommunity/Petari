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
    ActorStateBase(const char* pName, T* pActor) : ActorStateBaseInterface(pName), mHost(pActor) {}

    virtual ~ActorStateBase() NO_INLINE {}

    inline T* getHost() const { return  mHost;}
    T* mHost;
};
