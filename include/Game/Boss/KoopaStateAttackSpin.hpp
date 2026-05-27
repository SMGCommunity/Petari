#pragma once

#include "Game/Boss/Koopa.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"

class KoopaStateAttackSpin : public ActorStateBase< Koopa > {
public:
    KoopaStateAttackSpin(Koopa*);

    virtual ~KoopaStateAttackSpin();
    virtual void init();
    virtual void appear();
    virtual void kill();

    bool attackSensor(HitSensor*, HitSensor*);
    bool tryDamage(u32, HitSensor*, HitSensor*);

    void exeWaitToStart();
    void exeRun();
    void exeStart();
    void exeSpin();
    void exeEnd();
    void exeEven();

    /* 0x10 */ s32 _10;
    /* 0x14 */ s32 _14;
    /* 0x18 */ const MR::ActorMoveParam* _18;
    /* 0x1C */ u32 _1C;
};
