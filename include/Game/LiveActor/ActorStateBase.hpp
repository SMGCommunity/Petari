#pragma once

#include "Game/System/NerveExecutor.hpp"

class ActorStateBaseInterface : public NerveExecutor {
public:
    ActorStateBaseInterface(const char* pName) : NerveExecutor(pName) {
    }

    /* 0x08 */ virtual ~ActorStateBaseInterface() {
    }

    /* 0x0C */ virtual void init() {
    }

    /* 0x10 */ virtual void appear() {
        mIsDead = false;
    }

    /* 0x14 */ virtual void kill() {
        mIsDead = true;
    }

    /* 0x18 */ virtual bool update();

    /* 0x1C */ virtual void control() {
    }

    /* 0x8 */ bool mIsDead;
};

template < typename T >
class ActorStateBase : public ActorStateBaseInterface {
public:
    ActorStateBase(const char* pName, T* pActor) : ActorStateBaseInterface(pName), mHost(pActor) {
    }

    /* 0x08 */ virtual ~ActorStateBase() {
    }

    inline T* getHost() const {
        return mHost;
    }

    /* 0xC */ T* mHost;
};
