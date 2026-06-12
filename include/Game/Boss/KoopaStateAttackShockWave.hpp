#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class HitSensor;
class Koopa;

class KoopaStateAttackShockWave : public ActorStateBase< Koopa > {
public:
    KoopaStateAttackShockWave(Koopa* pKoopa);

    virtual ~KoopaStateAttackShockWave();
    virtual void init();
    virtual void appear();
    virtual void kill();

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
