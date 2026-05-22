#pragma once

#include "Game/Boss/Koopa.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"

class KoopaFigureBall;

class KoopaStateAttackRoll : public ActorStateBase< Koopa > {
public:
    KoopaStateAttackRoll(Koopa*);

    virtual ~KoopaStateAttackRoll();
    virtual void init();
    virtual void appear();
    virtual void kill();

    bool tryCalcAndSetBaseMtx();
    bool attackSensor(HitSensor*, HitSensor*);
    bool isDamage(u32, HitSensor*, HitSensor*) const;
    bool isEnableGuard() const;

    void exeWaitToStart();
    void exeStart();
    void exeRollAir();
    void exeRollGround();
    void exeEndAir();
    void exeEndLand();

private:
    /* 0x10 */ KoopaFigureBall* mFigureBall;
    /* 0x14 */ s32 _14;
    /* 0x18 */ u32 _18;
    /* 0x1C */ u8 _1C;
    /* 0x1D */ u8 _1D;
};
