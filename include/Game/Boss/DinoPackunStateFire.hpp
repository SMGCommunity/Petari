#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class DinoPackun;

class DinoPackunStateFire : public ActorStateBase< DinoPackun > {
public:
    DinoPackunStateFire(DinoPackun*);

    virtual ~DinoPackunStateFire();
    virtual void init();
    virtual void appear();

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
    bool isFire() const;
    bool isEnableFire() const;
    bool isEnableCool() const;
};
