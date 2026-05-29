#pragma once

#include "Game/Boss/Koopa.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"

class KoopaStateJumpAway : public ActorStateBase< Koopa > {
public:
    KoopaStateJumpAway(Koopa*);

    virtual ~KoopaStateJumpAway();
    virtual void init();
    virtual void appear();

    void exeJumpStart();
    void exeJump();
    void exeLand();
};
