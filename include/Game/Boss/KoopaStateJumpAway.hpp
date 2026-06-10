#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Koopa;

class KoopaStateJumpAway : public ActorStateBase< Koopa > {
public:
    KoopaStateJumpAway(Koopa* pKoopa);

    virtual ~KoopaStateJumpAway();
    virtual void init();
    virtual void appear();

    void exeJumpStart();
    void exeJump();
    void exeLand();
};
