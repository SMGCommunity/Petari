#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class DinoPackun;

class DinoPackunStateAwake : public ActorStateBase< DinoPackun > {
public:
    DinoPackunStateAwake(DinoPackun*);

    virtual ~DinoPackunStateAwake();
    virtual void appear();
    virtual void kill();

    void exeFind();
};
