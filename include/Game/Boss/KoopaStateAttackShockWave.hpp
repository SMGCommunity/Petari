#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class HitSensor;
class Koopa;

class KoopaStateAttackShockWave : public ActorStateBase< Koopa > {
public:
    KoopaStateAttackShockWave(Koopa* pKoopa);

    /* 0x08 */ virtual ~KoopaStateAttackShockWave();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void appear();
    /* 0x14 */ virtual void kill();

    bool attackSensor(HitSensor* pSender, HitSensor* pReceiver);

    void exeJumpStart();
    void exeJump();
    void exeLand();
    void exeJumpFast();

    /* 0x10 */ s32 mMaxAttacks;
    /* 0x14 */ s32 mAttacks;
    /* 0x18 */ f32 mJumpVelocity;
    /* 0x1C */ f32 mGravity;
    /* 0x20 */ s32 mJumpDelay;
};
