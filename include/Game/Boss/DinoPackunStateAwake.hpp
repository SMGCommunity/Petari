#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class DinoPackun;

class DinoPackunStateAwake : public ActorStateBase< DinoPackun > {
public:
    DinoPackunStateAwake(DinoPackun*);

    /* 0x08 */ virtual ~DinoPackunStateAwake();
    /* 0x10 */ virtual void appear();
    /* 0x14 */ virtual void kill();

    void exeFind();
};
