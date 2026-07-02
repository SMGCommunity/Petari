#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class HitSensor;
class Koopa;
class KoopaFigureBall;

class KoopaStateChaseRoll : public ActorStateBase< Koopa > {
public:
    KoopaStateChaseRoll(Koopa* pKoopa);

    virtual ~KoopaStateChaseRoll();
    virtual void init();
    virtual void appear();
    virtual void kill();

    bool tryCalcAndSetBaseMtx();
    bool attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    bool tryDamage(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    bool isEnableGuard() const;

    void exeWaitToStart();
    void exeStart();
    void exeRollAir();
    void exeRollGround();
    void exeEndAir();
    void exeEndLand();

private:
    /* 0x10 */ KoopaFigureBall* mFigureBall;
    /* 0x14 */ s32 mRollDelay;
};
