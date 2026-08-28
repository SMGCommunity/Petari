#pragma once

#include "Game/Boss/PoltaActionBase.hpp"

class PoltaStateStagger;
class PoltaStateGroundRockAttack;
class PoltaStateGenerateRock;

class PoltaBattleLv1 : public PoltaActionBase {
public:
    PoltaBattleLv1(Polta*);

    /* 0x10 */ virtual void appear();
    /* 0x1C */ virtual void control();
    /* 0x20 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x24 */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x28 */ virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    bool isEnableSensor(const HitSensor*) const;
    bool isEnableDamage() const;
    bool isEnableScream() const NO_INLINE;

    void exeWait();
    void exeBreakBody();
    void exeStagger();
    void exeDamageCore();
    void addDamageBody();
    void exeGenerateRock();
    void exeGenerateGroundRock();
    void exeDamageBody();
    void endStagger();

    /* 0x14 */ PoltaStateStagger* mStateStagger;
    /* 0x18 */ PoltaStateGenerateRock* mStateGenerateRock;
    /* 0x1C */ PoltaStateGroundRockAttack* mStateGroundRockAttack;
    /* 0x20 */ s32 mPoltaHealth;
    s32 _24;
};
