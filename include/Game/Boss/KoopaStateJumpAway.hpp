#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Koopa;

class KoopaStateJumpAway : public ActorStateBase< Koopa > {
public:
    KoopaStateJumpAway(Koopa* pKoopa);

    /* 0x08 */ virtual ~KoopaStateJumpAway();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void appear();

    void exeJumpStart();
    void exeJump();
    void exeLand();
};
