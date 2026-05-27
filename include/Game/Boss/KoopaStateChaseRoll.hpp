#pragma once

#include "Game/Boss/Koopa.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"

class KoopaFigureBall;

class KoopaStateChaseRoll : public ActorStateBase< Koopa > {
public:
    KoopaStateChaseRoll(Koopa*);

    virtual ~KoopaStateChaseRoll();
    virtual void init();
    virtual void appear();
    virtual void kill();

    bool tryCalcAndSetBaseMtx();
    bool attackSensor(HitSensor*, HitSensor*);
    bool tryDamage(u32, HitSensor*, HitSensor*);
    bool isEnableGuard() const;

    void exeWaitToStart();
    void exeStart();
    void exeRollAir();
    void exeRollGround();
    void exeEndAir();
    void exeEndLand();

private:
    /* 0x10 */ KoopaFigureBall* mKoopaFigureBall;
    /* 0x14 */ s32 _14;
};
