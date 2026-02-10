#pragma once

#include "Game/Boss/PoltaActionBase.hpp"

class PoltaStateStagger;
class PoltaStateGroundRockAttack;
class PoltaStateGenerateRock;
class PoltaStateAttackGround;
class PoltaStatePunch;

class PoltaBattleLv2 : public PoltaActionBase {
public:
    PoltaBattleLv2(Polta*);

    virtual void appear();
    virtual void control();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    bool tryAttackGround();
    void setNerveGenerateRock();
    bool tryGenerateRock();

    void exeBreakBody();
    void exeStagger();
    void exeDamageCore();
    void endStagger();
    void exeDamageBody();
    void exeGenerateRock();
    void exeGenerateGroundRock();
    void exeAttackGround();
    void exePunch();
    void exeWait();

    bool isEnableSensor(const HitSensor*) const;
    bool isEnableAttack(const HitSensor*) const;
    bool isEnableDamage() const;
    bool isEnableScream() const NO_INLINE;
    void addDamageBody();

    /* 0x14 */ PoltaStateAttackGround* mStateAttackGround;
    /* 0x18 */ PoltaStatePunch* mStatePunch;
    /* 0x1C */ PoltaStateStagger* mStateStagger;
    /* 0x20 */ PoltaStateGenerateRock* mStateGenerateRock;
    /* 0x24 */ PoltaStateGroundRockAttack* mStateGroundRockAttack;
    /* 0x28 */ s32 mPoltaHealth;
    s32 _2C;
};
