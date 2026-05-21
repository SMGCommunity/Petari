#pragma once

#include "Game/Boss/Koopa.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"

class KoopaStateAttackHipDrop : public ActorStateBase< Koopa > {
public:
    KoopaStateAttackHipDrop(Koopa*);

    virtual ~KoopaStateAttackHipDrop();
    virtual void init();
    virtual void appear();
    virtual void kill();
    
    bool attackSensor(HitSensor*, HitSensor*);
    bool isDamage() const;
    bool isEnableGuard() const;

    void exeRun();
    void exeJumpStart();
    void exeJumpUp();
    void exeJumpWaitAir();
    void exeJumpAttackDown();
    void exeLand();
    void exeDamage();

private:
    /* 0x10 */ TVec3f _10;
    /* 0x1C */ s32 _1C;
    /* 0x20 */ s32 _20;
    /* 0x24 */ u32 _24;
    /* 0x28 */ u32 _28;
};
