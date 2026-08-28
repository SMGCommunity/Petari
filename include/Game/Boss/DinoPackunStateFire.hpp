#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class DinoPackun;

class DinoPackunStateFire : public ActorStateBase< DinoPackun > {
public:
    DinoPackunStateFire(DinoPackun*);

    /* 0x08 */ virtual ~DinoPackunStateFire();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void appear();

    void exeNormal();
    void exeFireSign();
    void exeCool();
    void exeFire();
    void requestFire();
    void requestFireDirect();
    void requestFireSign();
    void requestCool();
    void setFire();
    void setFireDirect();
    bool isFire() const NO_INLINE;
    bool isEnableFire() const;
    bool isEnableCool() const;
};
