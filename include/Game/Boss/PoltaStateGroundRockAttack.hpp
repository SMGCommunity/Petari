#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Polta;

class PoltaStateGroundRockAttack : public ActorStateBase< Polta > {
public:
    PoltaStateGroundRockAttack(Polta*);

    /* 0x08 */ virtual ~PoltaStateGroundRockAttack();
    /* 0x10 */ virtual void appear();

    void exeSign();
    void exeGenerate();
    void exeEnd();

    s32 _10;
    bool _14;
};
