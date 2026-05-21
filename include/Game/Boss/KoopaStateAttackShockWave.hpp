#pragma once

#include "Game/Boss/Koopa.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"

class KoopaStateAttackShockWave : public ActorStateBase< Koopa > {
public:
    KoopaStateAttackShockWave(Koopa*);

    virtual ~KoopaStateAttackShockWave();
    virtual void init();
    virtual void appear();
    virtual void kill();

    bool attackSensor(HitSensor*, HitSensor*);

    void exeJumpStart();
    void exeJump();
    void exeLand();
    void exeJumpFast();

    /* 0x10 */ s32 _10;
    /* 0x14 */ s32 _14;
    /* 0x18 */ f32 mJumpVelocity;
    /* 0x1C */ f32 mGravity;
    /* 0x20 */ u32 mJumpDelay;
};
