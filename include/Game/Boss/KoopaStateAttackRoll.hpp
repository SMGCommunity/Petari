#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class HitSensor;
class Koopa;
class KoopaFigureBall;

class KoopaStateAttackRoll : public ActorStateBase< Koopa > {
public:
    KoopaStateAttackRoll(Koopa* pKoopa);

    virtual ~KoopaStateAttackRoll();
    virtual void init();
    virtual void appear();
    virtual void kill();

    bool tryCalcAndSetBaseMtx();
    bool attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    bool isDamage(u32 msg, HitSensor* pSender, HitSensor* pReceiver) const;
    bool isEnableGuard() const;

    void exeWaitToStart();
    void exeStart();
    void exeRollAir();
    void exeRollGround();
    void exeEndAir();
    void exeEndLand();

    /* 0x10 */ KoopaFigureBall* mFigureBall;
    /* 0x14 */ s32 mRollDelay;
    /* 0x18 */ s32 mRollTime;
    /* 0x1C */ bool _1C;
    /* 0x1D */ bool _1D;
};
