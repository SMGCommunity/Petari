#pragma once

#include "Game/System/NerveExecutor.hpp"

class ActorStateBaseInterface : public NerveExecutor
{
public:
    inline ActorStateBaseInterface(const char *pName) : NerveExecutor(pName) {

    }

    virtual inline ~ActorStateBaseInterface() {

    }

    virtual void init() {
        return;
    }

    virtual void appear() {
        mIsDead = 0;
    }

    virtual void kill() {
        mIsDead = 1;
    }

    virtual bool update();

    virtual void control() {
        return;
    }

    u8 mIsDead; // 0x8
};

template<typename T>
class ActorStateBase : public ActorStateBaseInterface {
public:
    inline ActorStateBase(const char *pName) : ActorStateBaseInterface(pName) {

    }

    virtual ~ActorStateBase() {

    }
};
